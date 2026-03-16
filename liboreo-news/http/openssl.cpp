#include "openssl.hpp"

#include <openssl/err.h>
#include <openssl/ssl.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <string>
#include <string_view>

namespace oreo::news::http {

namespace {

struct Url {
  bool https = false;
  std::string host;
  std::string port;
  std::string path;
};

[[nodiscard]] Url ParseUrl(const std::string &url) {
  Url result;
  std::string_view sv{url};

  if (sv.starts_with("https://")) {
    result.https = true;
    sv.remove_prefix(8);
  } else if (sv.starts_with("http://")) {
    result.https = false;
    sv.remove_prefix(7);
  } else {
    return {};
  }

  auto slash = sv.find('/');
  auto host_port = sv.substr(0, slash);
  result.path = slash != std::string_view::npos ? sv.substr(slash) : "/";

  auto colon = host_port.find(':');
  if (colon != std::string_view::npos) {
    result.host = std::string{host_port.substr(0, colon)};
    result.port = std::string{host_port.substr(colon + 1)};
  } else {
    result.host = std::string{host_port};
    result.port = result.https ? "443" : "80";
  }

  return result;
}

[[nodiscard]] int ConnectSocket(const std::string &host,
                                const std::string &port) {
  addrinfo hints{};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  addrinfo *res = nullptr;
  if (getaddrinfo(host.c_str(), port.c_str(), &hints, &res) != 0) {
    return -1;
  }

  int fd = -1;
  for (auto *p = res; p; p = p->ai_next) {
    fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (fd < 0) {
      continue;
    }
    if (connect(fd, p->ai_addr, p->ai_addrlen) == 0) {
      break;
    }
    close(fd);
    fd = -1;
  }

  freeaddrinfo(res);
  return fd;
}

[[nodiscard]] std::string BuildRequest(const std::string &host,
                                       std::string_view path) {
  std::string req;
  req.reserve(256);
  req += "GET ";
  req += path;
  req += " HTTP/1.1\r\nHost: ";
  req += host;
  req += "\r\nConnection: close\r\n\r\n";
  return req;
}

[[nodiscard]] std::string ReadPlain(int fd) {
  std::string response;
  char buf[4096];
  for (;;) {
    auto n = read(fd, buf, sizeof(buf));
    if (n <= 0) {
      break;
    }
    response.append(buf, static_cast<std::size_t>(n));
  }
  return response;
}

[[nodiscard]] std::string ReadTls(SSL *ssl) {
  std::string response;
  char buf[4096];
  for (;;) {
    auto n = SSL_read(ssl, buf, sizeof(buf));
    if (n <= 0) {
      break;
    }
    response.append(buf, static_cast<std::size_t>(n));
  }
  return response;
}

[[nodiscard]] std::string_view ExtractBody(std::string_view response) {
  auto sep = response.find("\r\n\r\n");
  if (sep == std::string_view::npos) {
    return {};
  }
  return response.substr(sep + 4);
}

} // namespace

std::string OpenSSLClient::Get(const std::string &url) {
  auto parsed = ParseUrl(url);
  if (parsed.host.empty()) {
    return {};
  }

  int fd = ConnectSocket(parsed.host, parsed.port);
  if (fd < 0) {
    return {};
  }

  auto request = BuildRequest(parsed.host, parsed.path);
  std::string raw;

  if (parsed.https) {
    auto *ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
      close(fd);
      return {};
    }

    SSL_CTX_set_default_verify_paths(ctx);

    auto *ssl = SSL_new(ctx);
    if (!ssl) {
      SSL_CTX_free(ctx);
      close(fd);
      return {};
    }

    SSL_set_fd(ssl, fd);
    SSL_set_tlsext_host_name(ssl, parsed.host.c_str());

    if (SSL_connect(ssl) != 1) {
      SSL_free(ssl);
      SSL_CTX_free(ctx);
      close(fd);
      return {};
    }

    SSL_write(ssl, request.data(), static_cast<int>(request.size()));
    raw = ReadTls(ssl);

    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
  } else {
    auto remaining = request.size();
    const char *ptr = request.data();
    while (remaining > 0) {
      auto n = write(fd, ptr, remaining);
      if (n <= 0) {
        close(fd);
        return {};
      }
      ptr += n;
      remaining -= static_cast<std::size_t>(n);
    }
    raw = ReadPlain(fd);
  }

  close(fd);
  return std::string{ExtractBody(raw)};
}

} // namespace oreo::news::http

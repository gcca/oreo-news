#include "source.hpp"

#include "../http/openssl.hpp"
#include "news-y.hpp"
#include "rss.hpp"

namespace oreo::news {

Source::~Source() = default;

std::unique_ptr<Source> Source::MakeRSS(std::string url) {
  return std::make_unique<sources::RSSSource<http::OpenSSLClient>>(
      std::move(url), std::make_unique<http::OpenSSLClient>());
}

std::unique_ptr<Source> Source::MakeNewsY(std::uint8_t pages) {
  return std::make_unique<sources::NewsYSource<http::OpenSSLClient>>(pages);
}

} // namespace oreo::news

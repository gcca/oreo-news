#pragma once

#ifndef NDEBUG
#include <print>
#endif

#include <memory>
#include <string>

#include "../http/client.hpp"
#include "source.hpp"

namespace oreo::news::sources {

namespace detail {
void ParseRSS(const std::string &body);
} // namespace detail

template <http::Client ClientT>
class [[nodiscard]] RSSSource final : public Source {
public:
  RSSSource(std::string url, std::unique_ptr<ClientT> client)
      : url_{std::move(url)}, client_{std::move(client)} {}

  void Fetch() final {
    auto body = client_->Get(url_);
#ifndef NDEBUG
    std::println(stderr, "[RSS:Fetch] {} bytes from {:.60}", body.size(), url_);
#endif
    Parse(body);
  }

protected:
  void Parse(const std::string &body) final { detail::ParseRSS(body); }

private:
  std::string url_;
  std::unique_ptr<ClientT> client_;
};

} // namespace oreo::news::sources

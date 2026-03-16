#pragma once

#ifndef NDEBUG
#include <print>
#endif

#include <cstdint>
#include <memory>
#include <string>

#include "../http/client.hpp"
#include "source.hpp"

namespace oreo::news::sources {

namespace detail {
[[nodiscard]] std::string BuildPageUrl(std::uint8_t page);
} // namespace detail

template <http::Client ClientT>
class [[nodiscard]] PageYSource final : public Source {
public:
  PageYSource(std::uint8_t page, std::unique_ptr<ClientT> client)
      : page_{page}, client_{std::move(client)} {}

  void Fetch() final {
    auto url = detail::BuildPageUrl(page_);
    auto body = client_->Get(url);
#ifndef NDEBUG
    std::println(stderr, "[PageY:Fetch] {} bytes from {}", body.size(), url);
#endif
    Parse(body);
  }

protected:
  void Parse([[maybe_unused]] const std::string &body) final {
#ifndef NDEBUG
    std::println(stderr, "[PageY:Parse] {:.960}", body);
#endif
  }

private:
  std::uint8_t page_;
  std::unique_ptr<ClientT> client_;
};

} // namespace oreo::news::sources

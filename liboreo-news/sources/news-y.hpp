#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "../http/client.hpp"
#include "compositor.hpp"
#include "page-y.hpp"

namespace oreo::news::sources {

template <http::Client ClientT>
class [[nodiscard]] NewsYSource final : public SourceCompositor {
public:
  explicit NewsYSource(std::uint8_t pages)
      : SourceCompositor{BuildPages(pages)} {}

private:
  [[nodiscard]] static std::vector<std::unique_ptr<Source>>
  BuildPages(std::uint8_t pages) {
    std::vector<std::unique_ptr<Source>> sources;
    sources.reserve(pages);
    for (std::uint8_t i = 0; i < pages; ++i) {
      sources.push_back(std::make_unique<PageYSource<ClientT>>(
          i, std::make_unique<ClientT>()));
    }
    return sources;
  }
};

} // namespace oreo::news::sources

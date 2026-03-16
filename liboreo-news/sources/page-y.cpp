#include "page-y.hpp"

namespace oreo::news::sources::detail {

std::string BuildPageUrl(std::uint8_t page) {
  return "https://news.ycombinator.com/?p=" +
         std::to_string(static_cast<unsigned>(page) + 1);
}

} // namespace oreo::news::sources::detail

#include "rss.hpp"

#ifndef NDEBUG
#include <print>
#endif

#include <pugixml.hpp>

namespace oreo::news::sources::detail {

void ParseRSS(const std::string &body) {
  pugi::xml_document doc;
  if (!doc.load_string(body.c_str())) {
#ifndef NDEBUG
    std::println(stderr,
                 "[RSS:Parse] failed to parse XML, first 120 chars: {:.120}",
                 body);
#endif
    return;
  }

  auto channel = doc.child("rss").child("channel");
#ifndef NDEBUG
  std::println(stderr, "[RSS:Parse] channel: {}", channel.child_value("title"));
#endif

  for ([[maybe_unused]] auto item : channel.children("item")) {
#ifndef NDEBUG
    std::println(stderr, "[RSS] {}: {}", item.child_value("title"),
                 item.child_value("link"));
#endif
  }
}

} // namespace oreo::news::sources::detail

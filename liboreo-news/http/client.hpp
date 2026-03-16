#pragma once

#include <concepts>
#include <string>

namespace oreo::news::http {

template <class T>
concept Client = requires(T t, const std::string &url) {
  { t.Get(url) } -> std::same_as<std::string>;
};

} // namespace oreo::news::http

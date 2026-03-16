#pragma once

#include <string>

namespace oreo::news::http {

class [[nodiscard]] OpenSSLClient final {
public:
  [[nodiscard]] std::string Get(const std::string &url);
};

} // namespace oreo::news::http

#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace oreo::news {

class Source {
public:
  virtual ~Source() = 0;

  virtual void Fetch() = 0;

  [[nodiscard]] static std::unique_ptr<Source> MakeRSS(std::string url);
  [[nodiscard]] static std::unique_ptr<Source> MakeNewsY(std::uint8_t pages);

  Source(const Source &) = delete;
  Source &operator=(const Source &) = delete;
  Source(Source &&) = delete;
  Source &operator=(Source &&) = delete;

protected:
  Source() = default;
  virtual void Parse(const std::string &body) = 0;
};

} // namespace oreo::news

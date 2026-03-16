#pragma once

#include <memory>
#include <vector>

namespace oreo::news {

class Source;

class Aggregator {
public:
  void Append(std::unique_ptr<Source> source);
  void FetchAll();

private:
  std::vector<std::unique_ptr<Source>> sources_;
};

} // namespace oreo::news

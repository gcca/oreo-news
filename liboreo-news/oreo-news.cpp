#include "oreo-news.hpp"

#include "sources/source.hpp"

namespace oreo::news {

void Aggregator::Append(std::unique_ptr<Source> source) {
  sources_.push_back(std::move(source));
}

void Aggregator::FetchAll() {
  for (auto &source : sources_) {
    source->Fetch();
  }
}

} // namespace oreo::news

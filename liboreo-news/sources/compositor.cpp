#include "compositor.hpp"

namespace oreo::news::sources {

SourceCompositor::SourceCompositor(std::vector<std::unique_ptr<Source>> sources)
    : sources_{std::move(sources)} {}

void SourceCompositor::Fetch() {
  for (auto &source : sources_) {
    source->Fetch();
  }
}

void SourceCompositor::Parse([[maybe_unused]] const std::string &body) {}

} // namespace oreo::news::sources

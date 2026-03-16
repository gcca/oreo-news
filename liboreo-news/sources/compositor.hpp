#pragma once

#include <memory>
#include <vector>

#include "source.hpp"

namespace oreo::news::sources {

class SourceCompositor : public Source {
public:
  explicit SourceCompositor(std::vector<std::unique_ptr<Source>> sources);
  void Fetch() final;

protected:
  void Parse(const std::string &body) final;

private:
  std::vector<std::unique_ptr<Source>> sources_;
};

} // namespace oreo::news::sources

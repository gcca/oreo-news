#include <gtest/gtest.h>

#include "../testing/mocks.hpp"
#include "news-y.hpp"

namespace oreo::news::sources {
namespace {

using MockNewsYSource = NewsYSource<testing::MockClient>;

TEST(NewsYSourceTest, ConstructsWithZeroPages) {
  MockNewsYSource source{0};
  source.Fetch();
}

TEST(NewsYSourceTest, ConstructsWithOnePage) { MockNewsYSource source{1}; }

} // namespace
} // namespace oreo::news::sources

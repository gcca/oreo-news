#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../testing/mocks.hpp"
#include "compositor.hpp"

namespace oreo::news::sources {
namespace {

using ::testing::NiceMock;

TEST(SourceCompositorTest, FetchDelegatesToChildren) {
  std::vector<std::unique_ptr<Source>> children;

  auto *raw1 = new NiceMock<testing::MockSource>();
  auto *raw2 = new NiceMock<testing::MockSource>();
  EXPECT_CALL(*raw1, Fetch()).Times(1);
  EXPECT_CALL(*raw2, Fetch()).Times(1);

  children.push_back(std::unique_ptr<Source>(raw1));
  children.push_back(std::unique_ptr<Source>(raw2));

  SourceCompositor compositor{std::move(children)};
  compositor.Fetch();
}

TEST(SourceCompositorTest, FetchEmptyChildren) {
  std::vector<std::unique_ptr<Source>> children;
  SourceCompositor compositor{std::move(children)};
  compositor.Fetch();
}

} // namespace
} // namespace oreo::news::sources

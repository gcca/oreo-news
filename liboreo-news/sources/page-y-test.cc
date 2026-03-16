#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../testing/mocks.hpp"
#include "page-y.hpp"

namespace oreo::news::sources {
namespace {

using ::testing::NiceMock;
using ::testing::Return;

TEST(PageYSourceTest, FetchBuildsCorrectUrlPage0) {
  auto client = std::make_unique<NiceMock<testing::MockClient>>();
  EXPECT_CALL(*client, Get("https://news.ycombinator.com/?p=1"))
      .WillOnce(Return("<html></html>"));

  PageYSource<NiceMock<testing::MockClient>> source{0, std::move(client)};
  source.Fetch();
}

TEST(PageYSourceTest, FetchBuildsCorrectUrlPage5) {
  auto client = std::make_unique<NiceMock<testing::MockClient>>();
  EXPECT_CALL(*client, Get("https://news.ycombinator.com/?p=6"))
      .WillOnce(Return("<html></html>"));

  PageYSource<NiceMock<testing::MockClient>> source{5, std::move(client)};
  source.Fetch();
}

TEST(PageYSourceTest, FetchBuildsCorrectUrlPage255) {
  auto client = std::make_unique<NiceMock<testing::MockClient>>();
  EXPECT_CALL(*client, Get("https://news.ycombinator.com/?p=256"))
      .WillOnce(Return("<html></html>"));

  PageYSource<NiceMock<testing::MockClient>> source{255, std::move(client)};
  source.Fetch();
}

} // namespace
} // namespace oreo::news::sources

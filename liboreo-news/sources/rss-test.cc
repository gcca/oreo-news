#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../testing/mocks.hpp"
#include "rss.hpp"

namespace oreo::news::sources {
namespace {

using ::testing::NiceMock;
using ::testing::Return;

constexpr auto kValidRSS = R"(<?xml version="1.0"?>
<rss version="2.0">
  <channel>
    <title>Test Feed</title>
    <item>
      <title>Article 1</title>
      <link>https://example.com/1</link>
      <description>Desc 1</description>
    </item>
    <item>
      <title>Article 2</title>
      <link>https://example.com/2</link>
      <description>Desc 2</description>
    </item>
  </channel>
</rss>)";

TEST(RSSSourceTest, FetchCallsClientGet) {
  auto client = std::make_unique<NiceMock<testing::MockClient>>();
  EXPECT_CALL(*client, Get("https://example.com/rss"))
      .WillOnce(Return(kValidRSS));

  RSSSource<NiceMock<testing::MockClient>> source{"https://example.com/rss",
                                                  std::move(client)};
  source.Fetch();
}

TEST(RSSSourceTest, FetchHandlesEmptyBody) {
  auto client = std::make_unique<NiceMock<testing::MockClient>>();
  EXPECT_CALL(*client, Get).WillOnce(Return(""));

  RSSSource<NiceMock<testing::MockClient>> source{"https://example.com/rss",
                                                  std::move(client)};
  source.Fetch();
}

TEST(RSSSourceTest, FetchHandlesInvalidXML) {
  auto client = std::make_unique<NiceMock<testing::MockClient>>();
  EXPECT_CALL(*client, Get).WillOnce(Return("<not-valid-xml"));

  RSSSource<NiceMock<testing::MockClient>> source{"https://example.com/rss",
                                                  std::move(client)};
  source.Fetch();
}

} // namespace
} // namespace oreo::news::sources

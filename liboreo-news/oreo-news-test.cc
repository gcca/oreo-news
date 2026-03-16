#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "oreo-news.hpp"
#include "testing/mocks.hpp"

namespace oreo::news {
namespace {

using ::testing::NiceMock;

TEST(AggregatorTest, AppendAndFetchAll) {
  Aggregator aggregator;

  auto source = std::make_unique<NiceMock<testing::MockSource>>();
  EXPECT_CALL(*source, Fetch()).Times(1);

  aggregator.Append(std::move(source));
  aggregator.FetchAll();
}

TEST(AggregatorTest, FetchAllEmpty) {
  Aggregator aggregator;
  aggregator.FetchAll();
}

TEST(AggregatorTest, FetchAllMultiple) {
  Aggregator aggregator;

  auto s1 = std::make_unique<NiceMock<testing::MockSource>>();
  auto s2 = std::make_unique<NiceMock<testing::MockSource>>();
  auto s3 = std::make_unique<NiceMock<testing::MockSource>>();
  EXPECT_CALL(*s1, Fetch()).Times(1);
  EXPECT_CALL(*s2, Fetch()).Times(1);
  EXPECT_CALL(*s3, Fetch()).Times(1);

  aggregator.Append(std::move(s1));
  aggregator.Append(std::move(s2));
  aggregator.Append(std::move(s3));
  aggregator.FetchAll();
}

} // namespace
} // namespace oreo::news

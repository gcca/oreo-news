#pragma once

#include <gmock/gmock.h>

#include "sources/source.hpp"

namespace oreo::news::testing {

class MockClient {
public:
  MOCK_METHOD(std::string, Get, (const std::string &url));
};

class MockSource : public Source {
public:
  MOCK_METHOD(void, Fetch, (), (final));
  MOCK_METHOD(void, Parse, (const std::string &body), (final));
};

} // namespace oreo::news::testing

cmake_minimum_required(VERSION 4.2)
project(liboreo-news LANGUAGES CXX)

find_package(OpenSSL REQUIRED)

add_library(oreo-news
  oreo-news.cpp
  sources/source.cpp
  sources/rss.cpp
  sources/compositor.cpp
  sources/page-y.cpp
  http/openssl.cpp
  3rdpaty/pugixml/pugixml.cpp
)
target_include_directories(oreo-news PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(oreo-news PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/3rdpaty/pugixml)
target_link_libraries(oreo-news PRIVATE OpenSSL::SSL OpenSSL::Crypto)

add_executable(oreo-news-cli oreo-news-cli.cc)
target_link_libraries(oreo-news-cli PRIVATE oreo-news)

option(OREO_NEWS_TEST "Enable tests" OFF)

if(OREO_NEWS_TEST)
  enable_testing()
  find_package(GTest REQUIRED)
  add_executable(oreo-news-test
    oreo-news-test.cc
    sources/compositor-test.cc
    sources/rss-test.cc
    sources/page-y-test.cc
    sources/news-y-test.cc
  )
  target_link_libraries(oreo-news-test PRIVATE
    oreo-news GTest::gtest_main GTest::gmock
  )
  add_test(NAME oreo-news-test COMMAND oreo-news-test)
endif()

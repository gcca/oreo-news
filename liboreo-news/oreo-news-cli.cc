#include "oreo-news.hpp"
#include "sources/source.hpp"

int main() {
  oreo::news::Aggregator aggregator;
  // aggregator.Append(oreo::news::Source::MakeRSS("https://hnrss.org/frontpage"));
  aggregator.Append(oreo::news::Source::MakeNewsY(1));
  aggregator.FetchAll();
  return 0;
}

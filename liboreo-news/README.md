# Oreo News

A news aggregation library that collects and processes content
from multiple sources into a unified feed.

```
  ╔═══════════════╗     ╔═══════════════╗     ╔═══════════════╗
  ║   RSS Feed    ║     ║  HN Page 1    ║     ║  HN Page 2    ║
  ╚═══════╤═══════╝     ╚═══════╤═══════╝     ╚═══════╤═══════╝
          │                     │                     │
          ▼                     ▼                     ▼
    ┌───────────┐         ┌───────────┐         ┌───────────┐
    │  Source   │         │  Source   │         │  Source   │
    │ (fetch +  │         │ (fetch +  │         │ (fetch +  │
    │  parse)   │         │  parse)   │         │  parse)   │
    └─────┬─────┘         └─────┬─────┘         └─────┬─────┘
          │                     │                     │
          │                     └──────────┬──────────┘
          │                                │
          │                                ▼
          │                     ┌──────────────────┐
          │                     │   Compositor     │
          │                     │  (delegates to   │
          │                     │    children)     │
          │                     └────────┬─────────┘
          │                              │
          └──────────────┬───────────────┘
                         │
                         ▼
                  ┌──────────────┐
                  │  Aggregator  │
                  │  (FetchAll)  │
                  └──────────────┘
```

## Overview

Oreo News provides a composable architecture for news aggregation:

- Define **sources** that know how to fetch and parse content
- Combine sources using the **compositor** pattern
- Use the **aggregator** to fetch all sources at once

## Usage

### Single RSS feed

```
  Aggregator
  └── RSSSource("https://example.com/feed.xml")
```

Fetches the RSS feed, parses the XML, and extracts articles
with their titles, links, and descriptions.

### Multiple HN pages

```
  Aggregator
  └── NewsYSource(3)
        ├── PageYSource(0)  ──▶  ?p=1
        ├── PageYSource(1)  ──▶  ?p=2
        └── PageYSource(2)  ──▶  ?p=3
```

Creates a compositor that fetches N pages from HN.
Page indexing starts at zero; URLs are offset by one.

### Mixed sources

```
  Aggregator
  ├── RSSSource("https://blog.example.com/rss")
  ├── RSSSource("https://news.example.org/feed")
  └── NewsYSource(5)
        ├── PageYSource(0)
        ├── PageYSource(1)
        ├── PageYSource(2)
        ├── PageYSource(3)
        └── PageYSource(4)
```

Sources of different types can be combined freely.
The aggregator treats them all uniformly.

## Source Lifecycle

Every source follows the same two-phase lifecycle:

```
  ┌─────────┐                  ┌─────────┐
  │  Fetch  │   HTTP GET       │  Parse  │   Process
  │         │ ════════════▶    │         │ ════════════▶  results
  │         │   raw body       │         │   (XML, HTML, etc.)
  └─────────┘                  └─────────┘
```

1. **Fetch** retrieves raw content from the network
2. **Parse** processes the raw content into structured data

## Adding a New Source

To add a new source type:

1. Create a new header in `sources/` with a template class
   that inherits from `Source`
2. Implement `Fetch()` and `Parse()`
3. Add a static factory method to `Source` (e.g., `MakeMySource`)
4. If the parse logic needs external libraries, put it in a
   `detail::` function in a companion `.cpp` file

```
  sources/
  ├── my-source.hpp       ◀── template class definition + Fetch/Parse
  ├── my-source.cpp       ◀── detail::ParseMyFormat() if needed
  └── my-source-test.cc
```

## Building

```
cmake -B build
cmake --build build
./build/oreo-news-cli
```

With tests:

```
cmake -B build -DOREO_NEWS_TEST=ON
cmake --build build
ctest --test-dir build
```

# Web Crawler

## Overview

This is a web crawler implementation written in C that retrieves web pages and saves them to a specified directory. The crawler takes a seed URL, a directory to store the pages, and a maximum depth to control the level of crawling.

## Features

- Parses command-line arguments to configure the crawling process.
- Normalizes URLs and checks for internal URLs.
- Saves crawled web pages to a specified directory in a structured format.
- Performs depth-limited crawling to avoid excessive data retrieval.

## Requirements

- C compiler (gcc recommended)
- CS50 library (libcs50-given.a)
- Valid URLs to crawl

## Assumptions

- The seed URL provided must be an internal URL (i.e., part of the same domain).
- The maximum depth should be between 0 and 10, inclusive.
- The specified directory for saving crawled pages must exist and be writable.

## How to Build

To build the crawler, navigate to the `crawler` directory and run:

```bash
make

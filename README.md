# Log-Structured Storage Engine

A high-performance key-value storage engine written in C++17, implementing LSM-tree (Log-Structured Merge-tree) architecture for optimized write throughput.

## Overview

This project implements a persistent key-value store similar to LevelDB/RocksDB, featuring:
- **Write-optimized architecture**: Sequential writes for 100x performance over random I/O
- **Crash recovery**: Write-ahead logging ensures data durability
- **Space efficiency**: Background compaction to reclaim storage
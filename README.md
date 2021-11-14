# tile-map-parallel-builder

Quadtree-based multi-thread tiled pyramid building algorithm.

## Core Concept

NOTE: The ___level___ is different from TMS zoom level. In this description file, it starts with zero on the tiles of the original raster image.

The core concept of the algorithm is never reading the same tile twice from the disk. Based on that, with a little more consumption of memory, it eliminates the reading actions (from the disk) when merging lower-level tiles into an upper-level one. Building tiled pyramid is an IO-intensive task, so the actual speed-up is remarkable.

Tiles from each level are stored in a tile stack. The TileMapBuilder class scans a square region, which has a side length of 2^n tiles, in the __Z-curve order__. It is checked recursively that, as soon as there are four tiles from the same level on top of the tile stack, TileMapBuilder pops them out and try to merge them into a upper-level tile using resampling methods like Bilinear Interpolation. The four tiles will then be written to disk and their merging product will be pushed into the stack. TileMapBuilder guarantees that no more than three tiles from the same level live in the memory at the same time. This is how it keeps memory consumption in a controllable and acceptable range.

## Parallel Strategy

The entile tiling task is divided into several TileMapBuilders' work. TileMapScheduler class determines the best logical pyramid level to assign the building tasks according to threads available. Each of the TileMapBuilder takes care of a tile in the best level(builder level) and all its sub tiles down the quadtree. When the builders returns, the TileMapScheduler launches another builder on the top level to finish building the tiled pyramid in a single thread, if the user-specified level is higher enough.

## Implementation Techniques

To be continued...

## Further thinking

What if implement it with Node.js? It seems that Node is expert in IO-intensive programming.

## Parallel building test results

Platform: My Laptop...
| Item | Info |
| :--- | :--- |
| CPU | Intel Core i7-9750H @ 2.60Hz 6 Cores 12 Threads |
| RAM | Samsung M471A1K43DB1-CTD 8 GB DDR4-2666 DDR4 SDRAM ___* 2___ |
| Disk | SAMSUNG MZVLB512HAJQ-000H1 (476 GB) (SSD) |
| OS | Windows 11 |
| Compiler | MSVC 2019 |


### GaoFen Six (Satellite) - Raster(resampled) 9.55GB - 60469 * 56563, 3 bands, 8 bit.
Building tiled pyramid to the top level.

| Tile Size | Threads Num | Elapsed Time(s) |
| --------- | ----------- | ------------ |
|           |   1         | 393.603      |
|           |   2         | 252.754      | 
|           |   4         | 192.305      | 
| 256 * 256 |   6         | 271.312      | 
|           |   8         | 276.798      | 
|           |   10        | 282.708      | 
|           |   12        | 258.922      | 


| Tile Size | Threads Num | Elapsed Time(s) |
| --------- | ----------- | ------------ |
|           |   1         | 187.004      |
|           |   2         | 130.351      | 
|           |   4         | 111.250      | 
| 512 * 512 |   6         | 120.803      | 
|           |   8         | 129.125      | 
|           |   10        | 243.630      | 
|           |   12        | 400.413      | 

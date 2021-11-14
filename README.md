# tile-map-parallel-builder

Quadtree-based multi-thread tiled pyramid building algorithm.

## Parallel building test results

Platform: My Laptop...
| Item | Info |
| :--- | :--- |
| CPU | Intel Core i7-9750H @ 2.60Hz 6 Cores 12 Threads |
| RAM | Samsung M471A1K43DB1-CTD 8 GB DDR4-2666 DDR4 SDRAM ___* 2___ |
| Disk | SAMSUNG MZVLB512HAJQ-000H1 (476 GB) (SSD) |
| OS | Windows 11 |
| Compiler | MSVC 2019 |


### GaoFen Six (Satellite) - Raster(resampled) 60469 * 56563, 3 bands, 8 bit.
Building tiled pyramid to the top level (originial raster is considered as level zero).

| Tile Size | Threads Num | Elapsed Time(s) |
| --------- | ----------- | ------------ |
|           |   1         | 393.603      |
|           |   2         | 252.754      | 
|           |   4         | 192.305      | 
| 256 * 256 |   6         | 271.312      | 
|           |   8         | 276.798      | 
|           |   10        | 282.708      | 
|           |   12        | 258.922      | 


| Tile Size | Threads Num | Elapsed Time |
| --------- | ----------- | ------------ |
|           |   1         | 187.004      |
|           |   2         | 130.351      | 
|           |   4         | 111.250      | 
| 512 * 512 |   6         | 120.803      | 
|           |   8         | 129.125      | 
|           |   10        | 243.630      | 
|           |   12        | 400.413      | 

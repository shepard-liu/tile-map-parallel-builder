list(APPEND CMAKE_PREFIX_PATH "D:/GDAL/release-1928-x64")

add_library(TileMap
        Tile.h
        TileMapBuilder.h
        TileDatasetAccessor.h
        TileResampler.h
        TileMapScheduler.h
        TileMemoryAccessor.h
        Tile.cpp
        TileMapBuilder.cpp
        TileDatasetAccessor.cpp
        TileResampler.cpp
        TileMapScheduler.cpp
        TileMemoryAccessor.cpp TileAccessor.cpp TileAccessor.h)

# Configuring GDAL Support
find_library(GDAL_LIB gdal_i)
find_path(GDAL_INCLUDE gdal_priv.h)

# Configuring OpenMP Support
find_package(OpenMP)
if (OpenMP_CXX_FOUND)
    target_link_libraries(TileMap PUBLIC OpenMP::OpenMP_CXX)
endif ()

target_link_libraries(TileMap PUBLIC "${GDAL_LIB}")
target_include_directories(TileMap PUBLIC "${GDAL_INCLUDE}")
#ifndef TILE_MAP_PARALLEL_TILEMAPSCHEDULER_H
#define TILE_MAP_PARALLEL_TILEMAPSCHEDULER_H

#include <string>
#include "presets.h"
#include "TileDatasetAccessor.h"

/**
 * TileMapScheduler first calculates the tiles information of
 * each level using TileDatasetAccessor(TilePyramidLevel). Then breaks
 * an appropriate builderLevelIndex into several independent tiles according
 * to the maximum available threads number, enabling the TileMapBuilder
 * instance to build the tiled pyramid for each tile region. Finally it
 * completes the tiled pyramid over that builderLevelIndex in a single thread.
 */
class TileMapScheduler {

//---------------------
//  LifeCycle
//---------------------
public:
    /**
     * Constructor
     * @param _datasetPath      File path of the raster dataset
     * @param _tileMapDir       Directory to store built tiles
     * @param _maxThreadsNum    Number of threads to parallel
     * @param _topLevelIndex    The highest level index of the tiled pyramid to build. Level number begins with zero.
     * @param _tileWidth        Width of the tile.
     * @param _tileHeight       Height of the tile.
     */
    explicit TileMapScheduler(const std::string &_datasetPath, const std::string &_tileMapDir,
                              int _maxThreadsNum, int _topLevelIndex, int _tileWidth, int _tileHeight);

    /**
     * Constructor.
     * Tile size will be defaulted to 256 * 256.
     * @param _datasetPath      File path of the raster dataset
     * @param _tileMapDir       Directory to store built tiles
     * @param _maxThreadsNum    Number of threads to parallel
     * @param _topLevelIndex    The highest level index of the tiled pyramid to build. Level number begins with zero.
     */
    explicit TileMapScheduler(const std::string &_datasetPath, const std::string &_tileMapDir, int _maxThreadsNum,
                              int _topLevelIndex) :
            TileMapScheduler(_datasetPath, _tileMapDir, _maxThreadsNum, _topLevelIndex,
                             DEFAULT_TILE_WIDTH, DEFAULT_TILE_HEIGHT) {}

    /**
     * Constructor.
     * Tile size will be defaulted to 256 * 256.
     * Will build the tiled pyramid as high as possible.
     * @param _datasetPath      File path of the raster dataset
     * @param _tileMapDir       Directory to store built tiles
     * @param _maxThreadsNum    Number of threads to parallel
     */
    explicit TileMapScheduler(const std::string &_datasetPath, const std::string &_tileMapDir, int _maxThreadsNum) :
            TileMapScheduler(_datasetPath, _tileMapDir, _maxThreadsNum, -1) {}

    /**
     * Constructor.
     * Tile size will be defaulted to 256 * 256.
     * Will build the tiled pyramid as high as possible.
     * Will parallel threads as many as possible.
     * @param _datasetPath      File path of the raster dataset
     * @param _tileMapDir       Directory to store built tiles
     */
    explicit TileMapScheduler(const std::string &_datasetPath, const std::string &_tileMapDir) :
            TileMapScheduler(_datasetPath, _tileMapDir, -1) {}

    virtual ~TileMapScheduler();

//---------------------
//  Private Members
//---------------------
private:

    // Main accessor, will be copied to construct other accessors
    TileDatasetAccessor *pAccessor{nullptr};
    int maxThreadsNum{0};
    int topLevelIndex{-1};

//---------------------
//  Public Functions
//---------------------
public:
    void run();

};


#endif //TILE_MAP_PARALLEL_TILEMAPSCHEDULER_H

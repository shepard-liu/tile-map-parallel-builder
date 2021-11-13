#ifndef TILE_MAP_PARALLEL_TILEPYRAMIDLEVEL_H
#define TILE_MAP_PARALLEL_TILEPYRAMIDLEVEL_H

#include "presets.h"
#include "cmath"
#include <sstream>

/**
 * TilePyramidLevel stores the meta information for a
 * builderLevelIndex in the tile pyramid.
 */
class TilePyramidLevel {

//---------------------
//  LifeCycle
//---------------------
public:
    explicit TilePyramidLevel(int _levelIndex, int _levelWidth, int _levelHeight, int _tileWidth, int _tileHeight) :
        levelIndex(_levelIndex), levelWidth(_levelWidth), levelHeight(_levelHeight),
        tileWidth(_tileWidth), tileHeight(_tileHeight) {

        // Calculate tiles count
        tileNumX = static_cast<int>(ceil(levelWidth / (double) tileWidth));
        tileNumY = static_cast<int>(ceil(levelHeight / (double) tileHeight));

        // Calculate edge tile size
        edgeTileWidth = levelWidth - tileWidth * (tileNumX - 1);
        edgeTileHeight = levelHeight - tileHeight * (tileNumY - 1);
    }

    explicit TilePyramidLevel(int _level, int _levelWidth, int _levelHeight) :
            TilePyramidLevel(_level, _levelWidth, _levelHeight, DEFAULT_TILE_WIDTH, DEFAULT_TILE_HEIGHT) {}

//---------------------
//  Public Members
//---------------------
public:
    int levelWidth{}, levelHeight{};
    int tileWidth{}, tileHeight{};
    int edgeTileWidth{}, edgeTileHeight{};
    int tileNumX{}, tileNumY{};
    int levelIndex{};

//---------------------
//  Public Functions
//---------------------
public:
    std::string toString()const{
        std::stringstream ss;
        ss << "Level " << levelIndex << ": tileNumX = " << tileNumX << " tileNumY = " << tileNumY << '\n';
        return ss.str();
    }
};


#endif //TILE_MAP_PARALLEL_TILEPYRAMIDLEVEL_H


#ifndef TILE_MAP_PARALLEL_TILEMAPBUILDER_H
#define TILE_MAP_PARALLEL_TILEMAPBUILDER_H

#include "TileAccessor.h"
#include <stack>
#include <utility>

struct TileStackElement {
    Tile *pTile;
    int x{}, y{}, level{};

    TileStackElement(Tile *_pTile, int _x, int _y, int _level) :
            pTile(_pTile), x(_x), y(_y), level(_level) {}
};

/**
 * TileMapBuilder schedules the order that tiles get loaded into memory,
 * stack up to construct pyramid and get exported to disk.
 *
 * Every instance of TileMapBuilder is responsible for building <<< only one tile at
 * certain level and all its sub tiles down the pyramid >>>. Consequently, in order
 * to construct an object of TileMapBuilder, you need to specify which tile and
 * pyramid level the builder is on.
 *
 * Designed to be reused by TileMapScheduler at maximum efficiency, the TileMapBuilder
 * can be constructed with a topLevelIndex parameter. Builder instance will stop merging
 * tiles on the topLevelIndex pyramid level into upper-level tiles. Be aware that tiles
 * on that level will stay in the tileStack until manually popped out and released.
 * 
 * IMPORTANT Destructor of TileMapBuilder is not responsible for releasing the memory of
 * tiles in the tileStack. Explicitly call saveAndReleaseAllTiles() to achieve that. 
 */
class TileMapBuilder {

//---------------------
//  LifeCycle
//---------------------
public:
    /**
     * Construct a TileMapBuilder instance
     * @param _offsetX                  X position of the tile on builderLevelIndex
     * @param _offsetY                  Y position of the tile on builderLevelIndex
     * @param _builderLevelIndex        Pyramid level index that the builder is on
     * @param _topLevelIndex            Highest level index of the tiled pyramid the builder merges tiles to
     * @param _bottomLevelIndex         Level index on which the builder reads tiles
     * @param _pAccessor                Accessor of tiles
     */
    explicit TileMapBuilder(int _offsetX, int _offsetY, int _builderLevelIndex, int _topLevelIndex,
                            int _bottomLevelIndex,
                            const TileAccessor *_pAccessor);

    virtual ~TileMapBuilder();

//---------------------
//  Private Members
//---------------------
private:
    // The offset of tiles on Level zero
    int offsetX{}, offsetY{};

    int builderLevelIndex{};
    int topLevelIndex{};
    int bottomLevelIndex{};

    const TileAccessor *pAccessor{};
    std::stack<TileStackElement> tileStack;

//---------------------
//  Public Functions
//---------------------
public:
    //---------------------
    //  Getters
    //---------------------
    const std::stack<TileStackElement> &getTileStack() const { return tileStack; }

    std::stack<TileStackElement> &getTileStack() { return tileStack; }

    //---------------------
    //  Core Functions
    //---------------------
    void run();

    void saveAndReleaseAllTilesInStack();

//---------------------------
//  Static Public Functions
//---------------------------

};


#endif //TILE_MAP_PARALLEL_TILEMAPBUILDER_H

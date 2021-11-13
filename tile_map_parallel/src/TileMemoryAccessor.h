#ifndef TILE_MAP_PARALLEL_TILEMEMORYACCESSOR_H
#define TILE_MAP_PARALLEL_TILEMEMORYACCESSOR_H

#include "TileAccessor.h"

/**
 * TileMemoryAccessor inherits TileDatasetAccessor and provides the ability to
 * access tiles from the memory.
 */
class TileMemoryAccessor : public TileAccessor {
//---------------------
//  LifeCycle
//---------------------
public:
    /**
     * Constructor.
     * @param _tiles            A vector of tile pointers in Z-Curve order. nullptr is allowed.
     * @param _tileMapDir       The directory to save tile file.
     */
    explicit TileMemoryAccessor(const std::vector<Tile *> &_tiles, const std::vector<TilePyramidLevel> &_tileLevels,
                                const std::string &_tileMapDir);

    /**
     * Constructor.
     * @param _tiles            A vector of tile pointers in Z-Curve order. nullptr is allowed.
     * @param _tileMapDir       The directory to save tile file.
     * @param _tileWidth        Width of the tile.
     * @param _tileHeight       Height of the tile.
     */
    explicit TileMemoryAccessor(const std::vector<Tile *> &_tiles, const std::vector<TilePyramidLevel> &_tileLevels,
                                const std::string &_tileMapDir, int _tileWidth, int _tileHeight);

    // Copy constructor
    explicit TileMemoryAccessor(const TileMemoryAccessor& theOther);

    // Copy assignment constructor
    TileMemoryAccessor& operator=(const TileMemoryAccessor& theOther);

    ~TileMemoryAccessor() override;

//---------------------
//  Private Members
//---------------------
private:
    std::vector<Tile *> tiles;

//---------------------
//  Public Functions
//---------------------
public:
    /**
     * Read tile at position (posX, posY) from the raster dataset
     * The position is relative to the tile in the top left corner.
     * @param posX
     * @param posY
     * @return pointer of Tile object
     */
    Tile *getTileAt(int posX, int posY) const override;

//---------------------
//  Protected Functions
//---------------------
protected:
    void initAndCalcLevelMetas() override;

};


#endif //TILE_MAP_PARALLEL_TILEMEMORYACCESSOR_H

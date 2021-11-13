#ifndef TILE_MAP_PARALLEL_TILEACCESSOR_H
#define TILE_MAP_PARALLEL_TILEACCESSOR_H

#include <string>
#include <vector>
#include "presets.h"
#include "TilePyramidLevel.h"
#include "Tile.h"

/**
 * Base class of tile accessors.
 * TileAccessor enables the ability to access tiles from a raster source.
 */
class TileAccessor {

//---------------------
//  LifeCycle
//---------------------
public:
    explicit TileAccessor(std::string _tileMapDir, int _tileWidth, int _tileHeight);

    // Copy constructor
    explicit TileAccessor(const TileAccessor& theOther);

    // Copy assignment constructor
    TileAccessor& operator=(const TileAccessor& theOther);

    virtual ~TileAccessor() = default;

//---------------------
//  Private Members
//---------------------
protected:
    int tileWidth{}, tileHeight{};
    std::vector<TilePyramidLevel> tileLevels{};
    std::string tileMapDir{};

    GDALDriver* pDriver{ nullptr };

//---------------------
//  Public Functions
//---------------------
public:
    /**
     * Read tile at position (posX, posY) from the raster source
     * The position is relative to the tile in the top left corner.
     * @param posX
     * @param posY
     * @return pointer of Tile object
     */
    virtual Tile *getTileAt(int posX, int posY) const = 0;

    /**
     * Write tile to "tileMapDir/builderLevelIndex/posX/posY.tiff"
     * The position is relative to the tile in the top left corner.
     * @param pTile         The tile to save
     * @param posX          The X position of the tile in target pyramid builderLevelIndex
     * @param posY          The Y position of the tile in target pyramid builderLevelIndex
     * @param level         The pyramid builderLevelIndex of the tile
     */
    virtual void saveTile(const Tile *pTile, int posX, int posY, int level) const;

    //---------------------
    //  Getters
    //---------------------
    size_t getTopLevelIndex() const { return tileLevels.size() - 1; }

    const std::vector<TilePyramidLevel> &getLevelMetas() const { return tileLevels; }

    const std::string &getTileMapDir() const { return tileMapDir; }

    int getTileWidth() const { return tileWidth; }

    int getTileHeight() const { return tileHeight; }

//------------------------
//  Protected Functions
//------------------------
protected:
    /**
     * Implement this function to initialize the level meta information
     * when initializing a raster source.
     */
    virtual void initAndCalcLevelMetas() = 0;
};


#endif //TILE_MAP_PARALLEL_TILEACCESSOR_H

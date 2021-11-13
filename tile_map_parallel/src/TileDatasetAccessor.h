#ifndef TILE_MAP_PARALLEL_TILEDATASETACCESSOR_H
#define TILE_MAP_PARALLEL_TILEDATASETACCESSOR_H

#include <string>
#include "gdal_priv.h"
#include "TileAccessor.h"

/**
 * TileDatasetAccessor is a proxy class designed to provide transparent
 * accessing abilities for TileMapBuilder.
 *
 * TileDatasetAccessor handles the IO error when the tile accessed does
 * not exist. When reading/saving an edge tile, it will have the
 * standard size while part of its data may be set to NODATA value
 */
class TileDatasetAccessor : public TileAccessor {

//---------------------
//  LifeCycle
//---------------------
public:
    
    /**
     * Constructor
     * Tile size will be defaulted to 256 * 256
     * @param _datasetPath  Path of the raster dataset
     * @param _tileMapDir   Directory to store tiled map
     */
    explicit TileDatasetAccessor(const std::string &_datasetPath, const std::string &_tileMapDir);

    /**
     * Constructor.
     * @param _datasetPath  Path of the raster dataset.
     * @param _tileMapDir   Directory to store tiled map.
     * @param _tileWidth    Width of the tile.
     * @param _tileHeight   Height of the tile.
     */
    explicit TileDatasetAccessor(const std::string &_datasetPath, const std::string &_tileMapDir,
                                 int _tileWidth, int _tileHeight);

    // Copy constructor
    explicit TileDatasetAccessor(const TileDatasetAccessor& theOther);

    // Copy assignment constructor
    TileDatasetAccessor& operator=(const TileDatasetAccessor& theOther);

    ~TileDatasetAccessor() override;

//---------------------
//  Private Members
//---------------------
private:
    std::string datasetPath{};
    GDALDataset *pDataset{nullptr};

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

//------------------------
//  Protected Functions
//------------------------
protected:
    void initAndCalcLevelMetas() override;

//------------------------
//  Private Functions
//------------------------
private:
    void _open();

};


#endif //TILE_MAP_PARALLEL_TILEDATASETACCESSOR_H
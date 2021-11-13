#ifndef TILE_MAP_PARALLEL_TILE_H
#define TILE_MAP_PARALLEL_TILE_H

#include "gdal_priv.h"
#include "RasterMeta.h"
#include <string>
#include "presets.h"

/**
 * Tile is the core class of tile map building
 * The Tile size can be varied, for there are situations that the
 * tile model is not a perfect quadtree.
 * For most of the tiles in a tile map, sizes are the same.
 *
 * The TMS Standard size : 256 * 256
 *
 * Consider that raster datasets often include multiple bands, Tile class
 * reads and stores raster data in separate bands.
 */
class Tile {

//---------------------------
//  Definitions
//---------------------------
public:
    friend class TileResampler;

//---------------------------
//  LifeCycle
//---------------------------

public:
    explicit Tile();

    explicit Tile(int _width, int _height);

    virtual ~Tile();

//---------------------------
//  Private Members
//---------------------------

private:
    // Tile Information
    int width{0}, height{0}, bands{0};
    GDALDataType dataType{GDT_Byte};

    // Raster Data
    uchar **pBandData{nullptr};

//---------------------------
//  Public Functions
//---------------------------

public:
    //-------------------------
    //  Getters
    //-------------------------

    uchar **getDataPtr() const { return pBandData; }

    int getWidth() const { return width; }

    int getHeight() const { return height; }

    int getBandCount() const { return bands; }

    GDALDataType getDataType() const { return dataType; }

    //-------------------------
    //  Core functions
    //-------------------------

    /**
     * Region loader function. Reads raster data from a particular rectangle region specified
     * by top left point and its width and height in pixels.
     *
     * To load a tile, the size of the tile must be valid
     * @param   x_offset        Horizontal offset relative to top left corner i.e. start column index
     * @param   y_offset        Vertical offset relative to top left corner i.e. start row index
     * @param   x_span          Width of the area to be read
     * @param   y_span          Height of the area to be read
     * @param   pDataset        The GDAL dataset to read from
     */
    void load(int x_offset, int y_offset, int x_span, int y_span, GDALDataset *pDataset);

    /**
     * Write raster data to the disk by specifying the directory and image name.
     *
     * To write the tile to the disk, it must have been loaded properly.
     * @param   filePath    Where to save the tile file
     */
    void save(const std::string &filePath, GDALDriver* pDriver) const;

    bool isEmpty() const { return pBandData; }

//---------------------------
//  Private Functions
//---------------------------

private:
    void _allocate();

    void _cleanUp();
};

#endif //TILE_MAP_PARALLEL_TILE_H

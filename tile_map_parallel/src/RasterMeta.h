#ifndef TILE_MAP_PARALLEL_RASTERMETA_H
#define TILE_MAP_PARALLEL_RASTERMETA_H

#include "gdal_priv.h"
#include <cassert>

/**
 * Contains the meta information for raster dataset
 */
class RasterMeta {

public:
    int width{0}, height{0}, bands{0};
    GDALDataType dataType{GDT_Byte};

public:
    /**
     * Calculate width * height
     * @return The number of the raster's pixel
     */
    inline int getPixels() const { return width * height; }

public:
    explicit RasterMeta(int _width, int _height, int _bands, GDALDataType _dataType) :
            width(_width), height(_height), bands(_bands), dataType(_dataType) {
        assert(width > 0 && _height > 0 && _bands > 0);
        assert(_dataType >= 0 && _dataType < GDT_TypeCount);
    }

    explicit RasterMeta(GDALDataset *pDataset) {
        assert(pDataset);
        width = pDataset->GetRasterXSize();
        height = pDataset->GetRasterYSize();
        bands = pDataset->GetRasterCount();
        dataType = pDataset->GetRasterBand(1)->GetRasterDataType();
    }

};

#endif //TILE_MAP_PARALLEL_RASTERMETA_H

#include "Tile.h"
#include <cassert>

Tile::Tile() : Tile(DEFAULT_TILE_WIDTH, DEFAULT_TILE_HEIGHT) {}

Tile::Tile(int _width, int _height) :
        width(_width), height(_height) {
    assert(_width > 0 && _height > 0);
}

void Tile::load(int x_offset, int y_offset, int x_span, int y_span, GDALDataset *pDataset) {
    assert(x_offset >= 0 && y_offset >= 0 && pDataset);
    assert(width > 0 && height > 0);

    //--------Reading Raster meta information--------//

    bands = pDataset->GetRasterCount();
    dataType = pDataset->GetRasterBand(1)->GetRasterDataType();

    // Clean up if existing raster data and reallocate
    _allocate();

    //------Read raster data band-wise------//

    for (int i = 0; i < bands; ++i) {
        pDataset->GetRasterBand(i + 1)->RasterIO( 
                GF_Read,
                x_offset, y_offset, x_span, y_span,
                pBandData[i],
                width, height,
                dataType,
                0, 0);
    }
}

void Tile::save(const std::string &filePath, GDALDriver* pDriver) const {
    assert(pBandData);
    assert(width > 0 && height > 0 && bands > 0);
    assert(dataType > 0 && dataType < GDT_TypeCount);
    assert(pDriver);

    GDALDataset *pOutDataSet = pDriver->Create(
            filePath.c_str(),
            width, height,
            bands,
            dataType,
            nullptr);

    for (int i = 0; i < bands; ++i) {
        assert(pBandData[i]);
        pOutDataSet->GetRasterBand(i + 1)->RasterIO(
                GF_Write,
                0, 0, width, height,
                pBandData[i],
                width, height,
                dataType,
                0, 0);
    }

    GDALClose(pOutDataSet);
}

void Tile::_allocate() {
    assert(width > 0 && height > 0 && bands > 0);
    assert(dataType > 0 && dataType < GDT_TypeCount);
    if (pBandData)
        _cleanUp();
    pBandData = new uchar *[bands]{};
    int pixels = width * height;
    for (int i = 0; i < bands; ++i)
        pBandData[i] = new uchar[pixels * GDALGetDataTypeSizeBytes(dataType)]{};
}

void Tile::_cleanUp() {
    if (!pBandData)
        return;
    assert(bands > 0);
    for (int i = 0; i < bands; ++i) {
        if (pBandData[i])
            delete[] pBandData[i];
    }
    delete[] pBandData;
    pBandData = nullptr;
}

Tile::~Tile() {
    _cleanUp();
}

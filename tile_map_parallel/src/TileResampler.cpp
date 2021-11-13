
#include "TileResampler.h"

Tile *TileResampler::resampleAndMerge(const Tile **pLowerTiles, TileResampler::ResampleAlgorithm alg) {
    if (!pLowerTiles[0])return nullptr;

    switch (alg) {
        case Bilinear:
            return _bilinear(pLowerTiles);
            break;
        default:
            assert(0);
            break;
    }

    return nullptr;
}

Tile *TileResampler::_bilinear(const Tile *pLowerTiles[4]) {

    //----- allocate memory for merged tile -----//

    int tileWidth = pLowerTiles[0]->getWidth();
    int tileHeight = pLowerTiles[0]->getHeight();
    int bands = pLowerTiles[0]->getBandCount();
    GDALDataType dataType = pLowerTiles[0]->getDataType();

    Tile *pTile = new Tile(tileWidth, tileHeight);
    pTile->bands = bands;
    pTile->dataType = dataType;
    pTile->_allocate();

    //----- extract data pointer from all tiles -----//
    uchar **pLowerTilesBandData[4]{};
    for (int i = 0; i < 4; ++i) {
        if (!pLowerTiles[i])
            pLowerTilesBandData[i] = nullptr;
        else
            pLowerTilesBandData[i] = pLowerTiles[i]->getDataPtr();
    }

    //----- forcefully convert data type to process -----//

    switch (pLowerTiles[0]->dataType) {
        case GDT_Byte:
            bilinear_t<uchar>(pLowerTilesBandData, pTile->getDataPtr(), tileWidth, tileHeight, bands);
            break;
        case GDT_UInt16:
            bilinear_t<UINT16>(reinterpret_cast<unsigned short ***>(pLowerTilesBandData),
                               reinterpret_cast<unsigned short **>(pTile->getDataPtr()), tileWidth, tileHeight, bands);
            break;
        case GDT_Int16:
            bilinear_t<INT16>(reinterpret_cast<short ***>(pLowerTilesBandData),
                              reinterpret_cast<short **>(pTile->getDataPtr()), tileWidth, tileHeight, bands);
            break;
        case GDT_UInt32:
            bilinear_t<UINT32>(reinterpret_cast<unsigned int ***>(pLowerTilesBandData),
                               reinterpret_cast<unsigned int **>(pTile->getDataPtr()), tileWidth, tileHeight, bands);
            break;
        case GDT_Int32:
            bilinear_t<INT32>(reinterpret_cast<int ***>(pLowerTilesBandData),
                              reinterpret_cast<int **>(pTile->getDataPtr()), tileWidth, tileHeight, bands);
            break;
        case GDT_Float32:
            bilinear_t<FLOAT32>(reinterpret_cast<float ***>(pLowerTilesBandData),
                                reinterpret_cast<float **>(pTile->getDataPtr()), tileWidth, tileHeight, bands);
            break;
        case GDT_Float64:
            bilinear_t<FLOAT64>(reinterpret_cast<double ***>(pLowerTilesBandData),
                                reinterpret_cast<double **>(pTile->getDataPtr()), tileWidth, tileHeight, bands);
            break;
        default:
            assert(0);
            break;
    }

    return pTile;
}

template<typename T>
void TileResampler::bilinear_t(
        T ***pLowerTilesBandData,
        T **pTileBandData,
        int tileWidth,
        int tileHeight,
        int bandCount) {

    int sectionWidth = tileWidth / 2;
    int sectionHeight = tileHeight / 2;

    // loop over each band of merged tile
    for (int b = 0; b < bandCount; ++b) {
        // loop over each section of merged tile
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                // corresponding lower-builderLevelIndex tile
                T **pCurLowerTile = pLowerTilesBandData[j * 2 + i];
                if (!pCurLowerTile) continue;

                T *pCurLowerTileBand = pCurLowerTile[b];
                T *pMergedTileBand = pTileBandData[b];
                // loop over each scanline of the merged tile section
                for (int y = 0; y < sectionHeight; ++y) {
                    // get pointers
                    T *pMergedScanlinePixel =
                            pMergedTileBand + (j * sectionHeight + y) * tileWidth + (i * sectionWidth);
                    T *pMergedScanlineEnd = pMergedScanlinePixel + sectionWidth;
                    T *pLowerTileUpperScanlinePixel = pCurLowerTileBand + 2 * y * tileWidth;
                    T *pLowerTileLowerScanlinePixel = pLowerTileUpperScanlinePixel + tileWidth;

                    // loop over each pixel of the merged tile scanline
                    do {
                        // Calculate averge
                        double sum =
                            static_cast<double>(*(pLowerTileUpperScanlinePixel)) +
                            static_cast<double>(*(pLowerTileUpperScanlinePixel + 1)) +
                            static_cast<double>(*(pLowerTileLowerScanlinePixel)) +
                            static_cast<double>(*(pLowerTileLowerScanlinePixel + 1));
                        *(pMergedScanlinePixel) = round(sum / 4.0);
                        pLowerTileUpperScanlinePixel += 2;
                        pLowerTileLowerScanlinePixel += 2;
                    } while (++pMergedScanlinePixel != pMergedScanlineEnd);
                }
            }
        }
    }
}
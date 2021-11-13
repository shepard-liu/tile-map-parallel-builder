#ifndef TILE_MAP_PARALLEL_TILERESAMPLER_H
#define TILE_MAP_PARALLEL_TILERESAMPLER_H

#include <memory>
#include "Tile.h"

/**
 * TileResampler handles the resampling of four lower-builderLevelIndex tiles and
 * merging them into a higher one.
 *
 * The resampling algorithm is Bilinear.
 */
class TileResampler {

//---------------------
//  Definitions
//---------------------
public:
    enum ResampleAlgorithm {
        Bilinear,   // The same as averaging the four pixels
    };

//---------------------
//  LifeCycle
//---------------------
private:
    TileResampler() = default;

    virtual ~TileResampler() = default;

//-------------------------
//  Public Static Functions
//-------------------------
public:
    /**
     * Resample and merge the four lower-builderLevelIndex tiles using the given
     * resampling algorithm.
     * @param pLowerTiles   The four lower-builderLevelIndex tiles
     * @param alg           The resampling algorithm
     * @return  pointer to the merged tile
     */
    static Tile *resampleAndMerge(const Tile *pLowerTiles[4], ResampleAlgorithm alg);

//--------------------------
//  Private Static Functions
//--------------------------
private:
    static Tile *_bilinear(const Tile *pLowerTiles[4]);

private:
    template<typename T>
    static void bilinear_t(
            T ***pLowerTilesBandData,
            T **pTileBandData,
            int tileWidth,
            int tileHeight,
            int bandCount);

};


#endif //TILE_MAP_PARALLEL_TILERESAMPLER_H

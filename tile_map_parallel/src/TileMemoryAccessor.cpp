
#include "TileMemoryAccessor.h"
#include "../libmorton/morton.h"

TileMemoryAccessor::TileMemoryAccessor(const std::vector<Tile *> &_tiles,
                                       const std::vector<TilePyramidLevel> &_tileLevels, const std::string &_tileMapDir)
        : TileMemoryAccessor(_tiles, _tileLevels, _tileMapDir, DEFAULT_TILE_WIDTH, DEFAULT_TILE_HEIGHT) {}

TileMemoryAccessor::TileMemoryAccessor(const std::vector<Tile *> &_tiles,
                                       const std::vector<TilePyramidLevel> &_tileLevels, const std::string &_tileMapDir,
                                       int _tileWidth, int _tileHeight)
        : TileAccessor(_tileMapDir, _tileWidth, _tileHeight), tiles(_tiles) {
    this->tileLevels = _tileLevels;
    initAndCalcLevelMetas();    // Does nothing
}

TileMemoryAccessor::TileMemoryAccessor(const TileMemoryAccessor& theOther)
	: TileAccessor(theOther), tiles(theOther.tiles){}

TileMemoryAccessor& TileMemoryAccessor::operator=(const TileMemoryAccessor& theOther){

    if (this == &theOther)
        return *this;
    TileAccessor::operator=(theOther);
    tiles = theOther.tiles;
    return *this;
}

TileMemoryAccessor::~TileMemoryAccessor() {
    for (Tile *&pTile: tiles) {
        if (pTile) delete pTile;
        pTile = nullptr;
    }
}

void TileMemoryAccessor::initAndCalcLevelMetas() {}

Tile *TileMemoryAccessor::getTileAt(int posX, int posY) const {
    // Encode morton to find index of the tile
    uint_fast64_t tileIndex = libmorton::morton2D_64_encode(posX, posY);
    assert(tileIndex < tiles.size());
    return tiles[tileIndex];
}

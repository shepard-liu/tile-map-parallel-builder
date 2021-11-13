#include "TileDatasetAccessor.h"
#include <cassert>
#include <memory>

TileDatasetAccessor::TileDatasetAccessor(const std::string& _datasetPath, const std::string& _tileMapDir) :
    TileDatasetAccessor(_datasetPath, _tileMapDir, DEFAULT_TILE_WIDTH, DEFAULT_TILE_HEIGHT) {}

TileDatasetAccessor::TileDatasetAccessor(const std::string &_datasetPath, const std::string &_tileMapDir,
                                         int _tileWidth, int _tileHeight) :
    TileAccessor(_tileMapDir, _tileWidth, _tileHeight), datasetPath(_datasetPath) {
    initAndCalcLevelMetas();
}

TileDatasetAccessor::TileDatasetAccessor(const TileDatasetAccessor& theOther):
    TileAccessor(theOther), datasetPath(theOther.datasetPath){
    // Open a dataset to get a different handle object
    _open();
}

TileDatasetAccessor& TileDatasetAccessor::operator=(const TileDatasetAccessor& theOther)
{
    if (this == &theOther)
        return *this;
    TileAccessor::operator=(theOther);
    datasetPath = theOther.datasetPath;
    _open();
    return *this;
}

TileDatasetAccessor::~TileDatasetAccessor() {
    if (pDataset)
        GDALClose(pDataset);
}

Tile *TileDatasetAccessor::getTileAt(int posX, int posY) const {

    assert(posX >= 0 && posY >= 0);

    if (posX >= tileLevels.front().tileNumX || posY >= tileLevels.front().tileNumY)
        return nullptr;

    //----- Determine the actual size of the tile -----//

    int actualTileWidth{0}, actualTileHeight{0};

    if (posX == tileLevels.front().tileNumX - 1) actualTileWidth = tileLevels.front().edgeTileWidth;
    else actualTileWidth = tileWidth;

    if (posY == tileLevels.front().tileNumY - 1) actualTileHeight = tileLevels.front().edgeTileHeight;
    else actualTileHeight = tileHeight;

    //----- Create instance of Tile and load raster data -----//

    Tile *pTile = new Tile(tileWidth, tileHeight);

    pTile->load(
            posX * tileWidth,
            posY * tileHeight,
            actualTileWidth,
            actualTileHeight,
            pDataset);

    return pTile;
}

void TileDatasetAccessor::initAndCalcLevelMetas() {
    assert(tileWidth > 0 && tileHeight > 0);

    _open();

    // Read raster meta information
    int rasterWidth = pDataset->GetRasterXSize();
    int rasterHeight = pDataset->GetRasterYSize();

    // Calculate tile pyramid level meta information
    int curLevelWidth{rasterWidth}, curLevelHeight{rasterHeight};
    int curLevel{0};
    while (true) {
        tileLevels.emplace_back(curLevel++, curLevelWidth, curLevelHeight, tileWidth, tileHeight);
        if (curLevelWidth <= tileWidth && curLevelHeight <= tileHeight)
            break;
        curLevelHeight /= 2;
        curLevelWidth /= 2;
    } ;
}

void TileDatasetAccessor::_open()
{
    assert(!datasetPath.empty());

    // Open GDAL dataset
    this->pDataset = static_cast<GDALDataset*>(GDALOpen(datasetPath.c_str(), GA_ReadOnly));    
    
    assert(pDataset);
}

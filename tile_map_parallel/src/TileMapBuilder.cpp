
#include "TileMapBuilder.h"
#include "TileResampler.h"
#include "../libmorton/morton.h"
#include <sstream>
#include <omp.h>
#include <iostream>

TileMapBuilder::TileMapBuilder(int _offsetX, int _offsetY, int _builderLevelIndex, int _topLevelIndex,
                               int _bottomLevelIndex, const TileAccessor *_pAccessor) :
        builderLevelIndex(_builderLevelIndex), topLevelIndex(_topLevelIndex), bottomLevelIndex(_bottomLevelIndex),
        pAccessor(_pAccessor) {

    if (_topLevelIndex > _builderLevelIndex)
        topLevelIndex = builderLevelIndex;

    int scale = static_cast<int>(pow(2, _builderLevelIndex - _bottomLevelIndex));
    offsetX = scale * _offsetX;
    offsetY = scale * _offsetY;
}

TileMapBuilder::~TileMapBuilder() {}

void TileMapBuilder::run() {

    //----- Initialization -----//

    // Calculate the number of tiles,
    // The TileMapBuilder always takes care of a square region
    uint_fast64_t tilesTotal = static_cast<int>(pow(4, builderLevelIndex - bottomLevelIndex));

    // Merging flag for tiles of each builderLevelIndex
    // tileCount[0] --> bottom level tiles num
    // tileCounter[topLevelIndex - bottomLevelIndex] --> top level tiles num
    std::vector<int> tileCounter(topLevelIndex - bottomLevelIndex + 1, 0);

    //----- Traverse the tiles in Z-curve order -----//

    for (uint_fast64_t i = 0; i < tilesTotal; ++i) {

        // Morton decodes to get x, y
        uint_fast32_t x{}, y{};
        libmorton::morton2D_64_decode(i, x, y);

        // Push tile at (x + offsetX, y + offsetY)
        int posX = x + offsetX, posY = y + offsetY;
        tileStack.emplace(pAccessor->getTileAt(posX, posY), posX, posY, bottomLevelIndex);

        // Update counter for bottom level
        ++tileCounter[0];

        // Search the counter for merging
        for (decltype(tileCounter)::size_type k = 0; tileCounter[k] == 4 && k < topLevelIndex - bottomLevelIndex;) {
            // counter of the k-level tiles in the stack
            int &curLevelTileCounter = tileCounter[k];
            // pointers of the four lower-level tiles
            const Tile *pLowerTiles[4]{};
            // position and level info of the merged upper tile
            int upperX{}, upperY{}, upperLevel{};

            //---------------------------------------------------
            // Pop and save all four tiles in this level
            //---------------------------------------------------

            // Do not put decrement operator inside the while parenthesis in this case!
            // Decrement curLevelTileCounter the first time you access it!
            while (curLevelTileCounter) {
                // Get the top element
                const TileStackElement &topElem = tileStack.top();
                // Calculate position of merged tile
                if (--curLevelTileCounter == 0) {
                    upperX = topElem.x / 2;
                    upperY = topElem.y / 2;
                    upperLevel = topElem.level + 1;
                }

                // Save pointers of the lower four tiles
                pLowerTiles[curLevelTileCounter] = topElem.pTile;
                // Save the top tile to the disk and pop
                pAccessor->saveTile(topElem.pTile, topElem.x, topElem.y, topElem.level);
                tileStack.pop();
            }

            //------------------------------------------------------
            // Merge the lower-level tiles into an upper one
            //------------------------------------------------------

            Tile *pUpperTile = TileResampler::resampleAndMerge(pLowerTiles, TileResampler::Bilinear);
            // Push the upper tile into the stack
            tileStack.emplace(pUpperTile, upperX, upperY, upperLevel);
            // Update the counter
            ++tileCounter[++k];

            //----------------------------------------------------------
            // Release the allocated memory for lower-level tiles
            //----------------------------------------------------------

            for (const Tile *&pTile: pLowerTiles)
                delete pTile;
        }
    }

#ifndef NO_DEBUG
    std::stringstream ss;
    ss << "Builder on thread#" << omp_get_thread_num() << " tileCounter: ";
    for(auto& count:tileCounter){
        ss << count << ' ';
    }
    ss << '\n';
    std::cout << "\n" << ss.str() << "\n";
#endif // !NO_DEBUG

}

void TileMapBuilder::saveAndReleaseAllTilesInStack() {
    while (!tileStack.empty()) {
        TileStackElement &elem = tileStack.top();
        pAccessor->saveTile(elem.pTile, elem.x, elem.y, elem.level);
        delete elem.pTile;
        tileStack.pop();
    }
}
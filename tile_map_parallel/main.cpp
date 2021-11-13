

#include <iostream>
#include "src/TileMapScheduler.h"
#include <filesystem>
#include <chrono>

using Clock = std::chrono::high_resolution_clock;
#define DUARATION(__START__, __END__) std::chrono::duration_cast<std::chrono::milliseconds>(__END__ - __START__).count()

int main() {
    const std::string datasetPath = "D:\\RS_Files\\Affined_out.jpg";
    const std::string tileMapDir = "C:\\Users\\10579\\Desktop\\Cache\\tile_map_test_SINGLE";
    TileMapScheduler tileMapScheduler(datasetPath, tileMapDir, 1, -1);

    auto begin = Clock::now();
    tileMapScheduler.run();
    auto end = Clock::now();

    std::cout << "Elapsed time: " << DUARATION(begin, end);

    return 0;
}

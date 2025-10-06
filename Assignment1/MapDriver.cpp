#include "Map.h"
#include <iostream>
#include <vector>
#include <string>

/**
 * @brief Test function to load multiple map files, validate them, 
 * and display their contents.
 */
void testLoadMaps() {
    // List of map files to test (replace with your actual map file paths)
    std::vector<std::string> mapFiles = {
        "test_valid.map",
        "test_invalid.map"
    };

    for (const auto& file : mapFiles) {
        std::cout << "----------------------------------------\n";
        std::cout << "Loading map file: " << file << std::endl;

        MapLoader loader(file);
        Map* map = loader.load();

        if (!map) {
            std::cout << "[ERROR] Failed to load map file: " << file << std::endl;
            continue;
        }

        std::cout << *map << std::endl;

        // Validate map
        if (map->validate()) {
            std::cout << "[INFO] Map is valid!" << std::endl;
        } else {
            std::cout << "[ERROR] Map is INVALID!" << std::endl;
        }

        // Print continents and their territories
        std::cout << "Continents and their territories:" << std::endl;
        for (auto continent : *map->getContinents()) {
            std::cout << *continent << std::endl;
            for (auto territory : *continent->getTerritories()) {
                std::cout << "    " << *territory << std::endl;
            }
        }

        // Clean up dynamically allocated map
        delete map;
        map = nullptr;
    }
}

/**
 * @brief Main function to run map loading and validation tests.
 */
int main() {
    testLoadMaps();
    return 0;
}

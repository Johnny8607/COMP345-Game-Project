#include "Map.h"
#include <iostream>
#include <vector>
#include <string>

/**
 * @brief Tests the loading, validation, and display of multiple map files.
 *
 * This function attempts to:
 *  - Load each map file specified in the list.
 *  - Create a Map object using the MapLoader.
 *  - Validate the structure of each map (connected graph, continents, etc.).
 *  - Display the map's continents and territories.
 *
 * It also ensures proper cleanup of dynamically allocated memory after each test.
 */
void testLoadMaps() {
    // ---------------------------------------------------------
    // List of map files to test.
    // ---------------------------------------------------------
    std::vector<std::string> mapFiles = {
        "test_valid.txt",   // Expected: Valid, properly connected map
        "test_invalid.txt"  // Expected: Invalid, missing connections or errors
    };

    // Loop through each test map file
    for (const auto& file : mapFiles) {
        std::cout << "----------------------------------------\n";
        std::cout << "Loading map file: " << file << std::endl;

        // Create a MapLoader to read and parse the file
        MapLoader loader(file);

        // Load the map from the file
        Map* map = loader.load();

        // Check if map loading failed
        if (!map) {
            std::cout << "[ERROR] Failed to load map file: " << file << std::endl;
            continue;
        }

        // Display summary of the map (uses operator<< overload)
        std::cout << *map << std::endl;

        // ---------------------------------------------------------
        // Validate the loaded map
        // ---------------------------------------------------------
        if (map->validate()) {
            std::cout << "[INFO] Map is valid!" << std::endl;
        } else {
            std::cout << "[ERROR] Map is INVALID!" << std::endl;
        }

        // ---------------------------------------------------------
        // Display all continents and their territories
        // ---------------------------------------------------------
        std::cout << "Continents and their territories:" << std::endl;

        for (auto continent : *map->getContinents()) {
            // Print continent details
            std::cout << *continent << std::endl;

            // Print all territories in this continent
            for (auto territory : *continent->getTerritories()) {
                std::cout << "    " << *territory << std::endl;
            }
        }

        // ---------------------------------------------------------
        // Clean up allocated memory to prevent memory leaks
        // ---------------------------------------------------------
        delete map;
        map = nullptr;
    }
}

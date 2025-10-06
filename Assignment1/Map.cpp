#include "Map.h"
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <set>
#include <fstream>
#include <sstream>
#include <map>

// ================================
// Territory Implementation
// ================================

Territory::Territory(const std::string& name) 
    : name(name), armies(new int(0)), owner(nullptr), continent(nullptr), 
      adjacentTerritories(new std::vector<Territory*>()) {}

Territory::Territory(const Territory& other)
    : name(other.name), armies(new int(*other.armies)), owner(other.owner), continent(other.continent),
      adjacentTerritories(new std::vector<Territory*>(*other.adjacentTerritories)) {}

Territory& Territory::operator=(const Territory& other) {
    if (this != &other) {
        name = other.name;
        *armies = *other.armies;
        owner = other.owner;
        continent = other.continent;
        *adjacentTerritories = *other.adjacentTerritories;
    }
    return *this;
}

Territory::~Territory() {
    delete armies;
    delete adjacentTerritories;
}

std::string Territory::getName() const { return name; }
int Territory::getArmies() const { return *armies; }
Player* Territory::getOwner() const { return owner; }
Continent* Territory::getContinent() const { return continent; }
std::vector<Territory*>* Territory::getAdjacents() const { return adjacentTerritories; }

void Territory::setArmies(int armiesVal) { *armies = armiesVal; }
void Territory::setOwner(Player* newOwner) { owner = newOwner; }
void Territory::setContinent(Continent* newContinent) { continent = newContinent; }
void Territory::addAdjacent(Territory* territory) { adjacentTerritories->push_back(territory); }

std::ostream& operator<<(std::ostream& out, const Territory& t) {
    out << "Territory: " << t.name << " (Armies: " << *t.armies << ")";
    return out;
}

// ================================
// Continent Implementation
// ================================

Continent::Continent(const std::string& name, int bonus) 
    : name(name), controlValue(new int(bonus)), territories(new std::vector<Territory*>()) {}

Continent::Continent(const Continent& other)
    : name(other.name), controlValue(new int(*other.controlValue)), territories(new std::vector<Territory*>(*other.territories)) {}

Continent& Continent::operator=(const Continent& other) {
    if (this != &other) {
        name = other.name;
        *controlValue = *other.controlValue;
        *territories = *other.territories;
    }
    return *this;
}

Continent::~Continent() {
    delete controlValue;
    delete territories;
}

std::string Continent::getName() const { return name; }
int Continent::getControlValue() const { return *controlValue; }
std::vector<Territory*>* Continent::getTerritories() const { return territories; }

void Continent::addTerritory(Territory* t) { territories->push_back(t); }

std::ostream& operator<<(std::ostream& out, const Continent& c) {
    out << "Continent: " << c.name << " (Bonus: " << *c.controlValue << ")";
    return out;
}

// ================================
// Map Implementation
// ================================

Map::Map() 
    : continents(new std::vector<Continent*>()), territories(new std::vector<Territory*>()) {}

Map::Map(const Map& other)
    : continents(new std::vector<Continent*>()), territories(new std::vector<Territory*>()) {
    for (auto c : *other.continents) {
        continents->push_back(new Continent(*c));
    }
    for (auto t : *other.territories) {
        territories->push_back(new Territory(*t));
    }
}

Map& Map::operator=(const Map& other) {
    if (this != &other) {
        // Delete old pointers
        for (auto c : *continents) delete c;
        for (auto t : *territories) delete t;

        continents->clear();
        territories->clear();

        // Deep copy
        for (auto c : *other.continents) continents->push_back(new Continent(*c));
        for (auto t : *other.territories) territories->push_back(new Territory(*t));
    }
    return *this;
}

Map::~Map() {
    for (auto c : *continents) delete c;
    for (auto t : *territories) delete t;
    delete continents;
    delete territories;
}

std::vector<Continent*>* Map::getContinents() const { return continents; }
std::vector<Territory*>* Map::getTerritories() const { return territories; }

void Map::addContinent(Continent* c) { continents->push_back(c); }
void Map::addTerritory(Territory* t) { territories->push_back(t); }

bool Map::validate() const {
    return isConnectedGraph() && areContinentsConnected() && eachTerritoryInOneContinent();
}

// ================================
// Helper DFS functions
// ================================

static void dfsTerritory(Territory* start, std::set<Territory*>& visited) {
    std::stack<Territory*> stack;
    stack.push(start);

    while (!stack.empty()) {
        Territory* current = stack.top(); stack.pop();
        if (visited.find(current) != visited.end()) continue;
        visited.insert(current);
        for (auto neighbor : *current->getAdjacents()) {
            if (visited.find(neighbor) == visited.end()) stack.push(neighbor);
        }
    }
}

bool Map::isConnectedGraph() const {
    if (territories->empty()) return false;
    std::set<Territory*> visited;
    dfsTerritory(territories->at(0), visited);
    return visited.size() == territories->size();
}

static void dfsContinentTerritory(Territory* start, std::set<Territory*>& visited, Continent* continent) {
    std::stack<Territory*> stack;
    stack.push(start);

    while (!stack.empty()) {
        Territory* current = stack.top(); stack.pop();
        if (visited.find(current) != visited.end()) continue;
        visited.insert(current);
        for (auto neighbor : *current->getAdjacents()) {
            if (neighbor->getContinent() == continent && visited.find(neighbor) == visited.end())
                stack.push(neighbor);
        }
    }
}

bool Map::areContinentsConnected() const {
    for (auto continent : *continents) {
        std::vector<Territory*>* terrs = continent->getTerritories();
        if (terrs->empty()) return false;

        std::set<Territory*> visited;
        dfsContinentTerritory(terrs->at(0), visited, continent);
        if (visited.size() != terrs->size()) return false;
    }
    return true;
}

bool Map::eachTerritoryInOneContinent() const {
    std::set<Territory*> allTerritories;
    for (auto continent : *continents) {
        for (auto t : *continent->getTerritories()) {
            if (!allTerritories.insert(t).second) return false;
        }
    }
    for (auto t : *territories) {
        if (allTerritories.find(t) == allTerritories.end()) return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& out, const Map& m) {
    out << "Map with " << m.continents->size() << " continents and "
        << m.territories->size() << " territories.";
    return out;
}

// ================================
// MapLoader Implementation
// ================================

MapLoader::MapLoader(const std::string& filename) : filename(filename) {}
MapLoader::MapLoader(const MapLoader& other) : filename(other.filename) {}
MapLoader& MapLoader::operator=(const MapLoader& other) {
    if (this != &other) filename = other.filename;
    return *this;
}
MapLoader::~MapLoader() {}

Map* MapLoader::load() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open map file: " << filename << std::endl;
        return nullptr;
    }

    std::string line, section;
    Map* map = new Map();

    std::map<int, Territory*> territoryById;
    std::map<int, int> territoryToContinentId;  // Maps territory ID to its continent ID
    std::map<int, Continent*> continentById;    // Maps continent ID to Continent object

    int nextContinentId = 1; // Used to assign IDs to continents in the order they appear

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == ';') continue;

        if (line[0] == '[') {
            section = line;
            continue;
        }

        std::istringstream iss(line);

        if (section == "[continents]") {
            // Format: <continent_name>=<bonus>
            size_t eq = line.find('=');
            if (eq != std::string::npos) {
                std::string name = line.substr(0, eq);
                int bonus = std::stoi(line.substr(eq + 1));

                Continent* continent = new Continent(name, bonus);
                map->addContinent(continent);
                continentById[nextContinentId++] = continent;
            }
        }
        else if (section == "[countries]") {
            // Format: <id> <country_name> <continent_id> <x> <y>
            int id, continentId, x, y;
            std::string name;
            iss >> id >> name >> continentId >> x >> y;

            Territory* territory = new Territory(name);
            map->addTerritory(territory);
            territoryById[id] = territory;
            territoryToContinentId[id] = continentId;
        }
        else if (section == "[borders]") {
            int id; iss >> id;
            Territory* from = territoryById[id];
            int adj;
            while (iss >> adj) {
                from->addAdjacent(territoryById[adj]);
            }
        }
    }

    // ==============================
    // Link territories to continents
    // ==============================
    for (auto& [territoryId, territory] : territoryById) {
        int continentId = territoryToContinentId[territoryId];
        auto it = continentById.find(continentId);
        if (it == continentById.end()) {
            std::cerr << "[ERROR] Continent ID " << continentId 
                      << " not found for territory " << territory->getName() << std::endl;
            continue;
        }
        Continent* cont = it->second;
        territory->setContinent(cont);
        cont->addTerritory(territory);
    }

    file.close();
    std::cout << "[INFO] Successfully loaded map: " << filename << std::endl;
    return map;
}

std::ostream& operator<<(std::ostream& out, const MapLoader& ml) {
    out << "MapLoader reading file: " << ml.filename;
    return out;
}

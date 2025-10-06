#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <string>
#include <vector>

class Player; // Forward declaration (Player class will be added later)


class Continent; // Forward declare

// ================================
// Territory Class
// ================================
class Territory {
public:
    // Constructors & Destructor
    Territory(const std::string& name);
    Territory(const Territory& other);               // Copy constructor
    Territory& operator=(const Territory& other);    // Assignment operator
    ~Territory();                                    // Destructor

    // Getters
    std::string getName() const;
    int getArmies() const;
    Player* getOwner() const;
    Continent* getContinent() const;
    std::vector<Territory*>* getAdjacents() const;

    // Setters
    void setArmies(int armies);
    void setOwner(Player* owner);
    void setContinent(Continent* continent);
    void addAdjacent(Territory* territory);

    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& out, const Territory& t);

private:
    std::string name;
    int* armies;
    Player* owner;
    Continent* continent;
    std::vector<Territory*>* adjacentTerritories;
};

// ================================
// Continent Class
// ================================
class Continent {
public:
    // Constructors & Destructor
    Continent(const std::string& name, int bonus);
    Continent(const Continent& other);
    Continent& operator=(const Continent& other);
    ~Continent();

    // Getters
    std::string getName() const;
    int getControlValue() const;
    std::vector<Territory*>* getTerritories() const;

    // Add territory
    void addTerritory(Territory* t);

    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& out, const Continent& c);

private:
    std::string name;
    int* controlValue;
    std::vector<Territory*>* territories;
};

// ================================
// Map Class
// ================================
class Map {
public:
    // Constructors & Destructor
    Map();
    Map(const Map& other);
    Map& operator=(const Map& other);
    ~Map();

    // Getters
    std::vector<Continent*>* getContinents() const;
    std::vector<Territory*>* getTerritories() const;

    // Add methods
    void addContinent(Continent* c);
    void addTerritory(Territory* t);

    // Validation
    bool validate() const;

    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& out, const Map& m);

private:
    std::vector<Continent*>* continents;
    std::vector<Territory*>* territories;

    // Helper functions for validation
    bool isConnectedGraph() const;
    bool areContinentsConnected() const;
    bool eachTerritoryInOneContinent() const;
};

// ================================
// MapLoader Class
// ================================
class MapLoader {
public:
    MapLoader(const std::string& filename);
    MapLoader(const MapLoader& other);
    MapLoader& operator=(const MapLoader& other);
    ~MapLoader();

    Map* load(); // Builds and returns a Map object

    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& out, const MapLoader& ml);

private:
    std::string filename;
};

#endif
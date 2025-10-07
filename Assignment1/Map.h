#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <string>
#include <vector>

class Player;   // Forward declaration for Player (used as pointer in Territory)
class Continent; // Forward declaration for circular reference

// ================================
// Territory Class
// Represents a territory in the map that belongs to a continent.
// Each territory has a name, army count, owner, and adjacent territories.
// ================================
class Territory {
public:
    // Constructors & Destructor
    Territory(const std::string& name);                 // Default constructor
    Territory(const Territory& other);                  // Copy constructor
    Territory& operator=(const Territory& other);       // Assignment operator
    ~Territory();                                       // Destructor (cleans up dynamic members)

    // Getters
    std::string getName() const;                        // Returns the name of the territory
    int getArmies() const;                              // Returns the number of armies in this territory
    Player* getOwner() const;                           // Returns the player who owns this territory
    Continent* getContinent() const;                    // Returns the continent this territory belongs to
    std::vector<Territory*>* getAdjacents() const;      // Returns list of adjacent territories

    // Setters
    void setArmies(int armies);                         // Sets number of armies
    void setOwner(Player* owner);                       // Assigns an owner to this territory
    void setContinent(Continent* continent);            // Links this territory to a continent
    void addAdjacent(Territory* territory);             // Adds an adjacent territory (bidirectional link not enforced)

    // Stream insertion operator (for printing)
    friend std::ostream& operator<<(std::ostream& out, const Territory& t);

private:
    std::string name;                                   // Name of the territory
    int* armies;                                        // Pointer to army count (for ownership semantics)
    Player* owner;                                      // Pointer to the player who controls the territory
    Continent* continent;                               // Pointer to the continent this territory belongs to
    std::vector<Territory*>* adjacentTerritories;       // Adjacent territories (graph edges)
};

// ================================
// Continent Class
// Represents a continent, which groups multiple territories.
// Each continent has a name, a control bonus value, and a list of territories.
// ================================
class Continent {
public:
    // Constructors & Destructor
    Continent(const std::string& name, int bonus);      // Creates a continent with name and control bonus
    Continent(const Continent& other);                  // Copy constructor
    Continent& operator=(const Continent& other);       // Assignment operator
    ~Continent();                                       // Destructor (cleans up dynamically allocated members)

    // Getters
    std::string getName() const;                        // Returns continent name
    int getControlValue() const;                        // Returns continent's bonus control value
    std::vector<Territory*>* getTerritories() const;    // Returns list of territories in this continent

    // Adds a territory to the continent
    void addTerritory(Territory* t);

    // Stream insertion operator (for printing)
    friend std::ostream& operator<<(std::ostream& out, const Continent& c);

private:
    std::string name;                                   // Name of the continent
    int* controlValue;                                  // Pointer to the control bonus (for deep copying)
    std::vector<Territory*>* territories;               // List of territories that belong to this continent
};

// ================================
// Map Class
// Represents the entire game map, composed of continents and territories.
// Provides validation to ensure the map is well-formed and fully connected.
// ================================
class Map {
public:
    // Constructors & Destructor
    Map();                                              // Default constructor
    Map(const Map& other);                              // Copy constructor (deep copy)
    Map& operator=(const Map& other);                   // Assignment operator (deep copy)
    ~Map();                                             // Destructor (frees all dynamically allocated objects)

    // Getters
    std::vector<Continent*>* getContinents() const;     // Returns all continents
    std::vector<Territory*>* getTerritories() const;    // Returns all territories

    // Add methods
    void addContinent(Continent* c);                    // Adds a continent to the map
    void addTerritory(Territory* t);                    // Adds a territory to the map

    // Validates the integrity of the map structure
    // Returns true if:
    //  - The entire map is connected
    //  - Each continent is a connected subgraph
    //  - Each territory belongs to exactly one continent
    bool validate() const;

    // Stream insertion operator (for printing)
    friend std::ostream& operator<<(std::ostream& out, const Map& m);

private:
    std::vector<Continent*>* continents;                // List of continents
    std::vector<Territory*>* territories;               // List of territories

    // Helper functions used in validation
    bool isConnectedGraph() const;                      // Checks if the entire map is connected
    bool areContinentsConnected() const;                // Checks if each continent is connected internally
    bool eachTerritoryInOneContinent() const;           // Ensures no territory belongs to multiple continents
};

// ================================
// MapLoader Class
// Responsible for reading .map files and constructing Map objects.
// Supports files with [continents], [countries], and [borders] sections.
// ================================
class MapLoader {
public:
    MapLoader(const std::string& filename);             // Constructor taking the filename
    MapLoader(const MapLoader& other);                  // Copy constructor
    MapLoader& operator=(const MapLoader& other);       // Assignment operator
    ~MapLoader();                                       // Destructor

    // Parses the file and builds a Map object
    Map* load();

    // Stream insertion operator (for printing)
    friend std::ostream& operator<<(std::ostream& out, const MapLoader& ml);

private:
    std::string filename;                               // File path to be read
};

#endif // MAP_H

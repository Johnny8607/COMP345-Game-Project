#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>


#include "Map.h"
#include "Player.h"
#include "Cards.h"
#include "Deck.h"
#include "Hand.h"
#include "Orders.h"
#include "GameEngine.h"

Player* GameEngine::neutralPlayer = nullptr; // Define static member neutralPlayer

/**
 * Default constructor - initializes game engine to "start" state
 */
GameEngine::GameEngine() : currentState("start"), map(nullptr), deck(nullptr)
{
    std::cout << "GameEngine initialized to 'start' state." << std::endl;
}

/**
 * Copy constructor - performs deep copy
 */
GameEngine::GameEngine(const GameEngine &other) : currentState(other.getCurrentState()), map(nullptr), deck(nullptr)
{
    // Deep copy map if it exists
    if (other.map) {
        map = new Map(*other.map);
    }
    
    if (!other.players.empty()) { // Check if the vector is not empty
        for (Player* p : other.players) { // Iterate the object
            if (p) {
                players.push_back(new Player(*p));
            }
        }
    }
    
    // Deep copy deck
    if (other.deck) {
        deck = new Deck(*other.deck);
    }
    
    std::cout << "GameEngine copied." << std::endl;
}

/**
 * Destructor - cleans up resources
 */
GameEngine::~GameEngine()
{
    // Delete map
    delete map;
    map = nullptr;
    
    // Delete deck
    delete deck;
    deck = nullptr;
    
    // We must iterate it and delete its contents (the Player pointers).
    for (Player* p : players) {
        delete p;
    }
    players.clear(); // Clear the vector itself
}

/**
 * Assignment operator - assigns one GameEngine to another
 */
GameEngine &GameEngine::operator=(const GameEngine &other)
{
    if (this != &other)
    {
        // Deep copy state
        currentState = other.getCurrentState();
        
        // Delete existing map
        if (map) {
            delete map;
        }
        map = nullptr;
        if (other.map) {
            map = new Map(*other.map);
        }
        
        // Delete existing players' CONTENTS, then clear the vector.
        if (!players.empty()) {
            for (Player* p : players) {
                if (p) {
                    delete p;
                }
            }
            players.clear(); // Clear the vector
        }

        if (!other.players.empty()) {
            for (Player* p : other.players) {
                if (p) {
                    players.push_back(new Player(*p));
                }
            }
        }
        
        // Delete existing deck
        if (deck) {
            delete deck;
        }
        deck = nullptr;
        if (other.deck) {
            deck = new Deck(*other.deck);
        }
    }
    return *this;
}

/**
 * Stream insertion operator - outputs the current state
 */
std::ostream &operator<<(std::ostream &os, const GameEngine &engine)
{
    std::string upperState = engine.getCurrentState();
    std::transform(upperState.begin(), upperState.end(), upperState.begin(),
                   [](unsigned char c)
                   { return std::toupper(c); });

    os << "The current state of the game is: " << upperState;
    return os;
}

/**
 * Processes user commands and triggers state transitions
 */
void GameEngine::processCommand(const std::string &command)
{
    // Incorrect commands result in error message
    if (!isValidTransition(command))
    {
        std::cout << "Error: Invalid command '" << command
                  << "' for current state '" << currentState << "'." << std::endl;
        return;
    }

    /**
     * Control flow uses states as specified in the graph
     * Correct commands trigger state transitions
     */

    if (currentState == "start")
    {
        transition("map loaded");
    }
    else if (currentState == "map loaded")
    {
        if (command == "loadmap")
        {
            transition("map loaded");
        }
        else if (command == "validatemap")
        {
            transition("map validated");
        }
    }
    else if (currentState == "map validated")
    {
        if (command == "addplayer")
        {
            transition("players added");
        }
    }
    else if (currentState == "players added")
    {
        if (command == "addplayer")
        {
            transition("players added");
        }
        else if (command == "assigncountries")
        {
            transition("assign reinforcement");
        }
    }
    else if (currentState == "assign reinforcement")
    {
        if (command == "issueorder")
        {
            transition("issue orders");
        }
    }
    else if (currentState == "issue orders")
    {
        if (command == "issueorder")
        {
            transition("issue orders");
        }
        else if (command == "endissueorders")
        {
            transition("execute orders");
        }
    }
    else if (currentState == "execute orders")
    {
        if (command == "execorder")
        {
            transition("execute orders");
        }
        else if (command == "endexecorders")
        {
            transition("assign reinforcement");
        }
        else if (command == "win")
        {
            transition("win");
        }
    }
    else if (currentState == "win")
    {
        if (command == "play")
        {
            transition("start");
        }
        else if (command == "end")
        {
            transition("end");
        }
    }
}

/**
 * Validates if a command is valid for the current state
 * Part 5 Requirement: Validates commands according to state graph
 */
bool GameEngine::isValidTransition(const std::string &command) const
{
    // Part 5 Requirement: Commands are validated according to the state diagram
    if (currentState == "start" && command == "loadmap")
        return true;
    if (currentState == "map loaded" && (command == "loadmap" || command == "validatemap"))
        return true;
    if (currentState == "map validated" && command == "addplayer")
        return true;
    if (currentState == "players added" && (command == "addplayer" || command == "assigncountries"))
        return true;
    if (currentState == "assign reinforcement" && command == "issueorder")
        return true;
    if (currentState == "issue orders" && (command == "issueorder" || command == "endissueorders"))
        return true;
    if (currentState == "execute orders" && (command == "execorder" || command == "endexecorders" || command == "win"))
        return true;
    if (currentState == "win" && (command == "play" || command == "end"))
        return true;

    return false;
}

/**
 * Transitions to a new state and displays the transition
 * Part 5 Requirement: State transitions as specified in the graph
 */
void GameEngine::transition(const std::string &newState)
{
    std::cout << "Transitioning from '" << currentState
              << "' to '" << newState << "'." << std::endl;
    currentState = newState;
}

/**
 * Gets the current state
 * @return The current state as a string
 */
std::string GameEngine::getCurrentState() const
{
    return currentState;
}

/**
 * Helper function to get list of map files in current directory
 * @return Vector of map filenames
 */
std::vector<std::string> getMapFiles()
{
    std::vector<std::string> mapFiles;
    
    // Try to list files in current directory
    try {
        for (const auto& entry : std::filesystem::directory_iterator(".")) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                // Check if file has .map extension or common map file extensions
                if (filename.find(".map") != std::string::npos || 
                    filename.find(".txt") != std::string::npos ||
                    filename == "testmap.txt" || filename == "test_valid.txt" || filename == "test_invalid.txt") {
                    mapFiles.push_back(filename);
                }
            }
        }
    } catch (const std::exception& e) {
        // If filesystem operations fail, provide default map files
        mapFiles.push_back("testmap.txt");
        mapFiles.push_back("test_valid.txt");
        mapFiles.push_back("test_invalid.txt");
    }
    
    return mapFiles;
}


// Helper function to distribute territories fairly among players
void GameEngine::distributeTerritories()
{
    if (!map || players.empty()) {
        std::cout << "Error: Cannot distribute territories - map or players not initialized." << std::endl;
        return;
    }
    
    std::vector<Territory*>* territories = map->getTerritories();
    if (!territories || territories->empty()) {
        std::cout << "Error: No territories to distribute." << std::endl;
        return;
    }
    
    // Shuffle territories randomly and equally
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(territories->begin(), territories->end(), gen);
    
    // Distribute territories round-robin style
    size_t playerIndex = 0;
    for (Territory* territory : *territories) {
        if (territory && playerIndex < players.size()) {
            Player* player = players[playerIndex];
            if (player) {
                player->addTerritory(territory);
                territory->setOwner(player);
                playerIndex = (playerIndex + 1) % players.size();
            }
        }
    }
    
    std::cout << "Territories distributed fairly and equally among " << players.size() << " players." << std::endl;
}


// Helper function to shuffle player order randomly
void GameEngine::shufflePlayerOrder()
{
    if (players.empty()) {
        return; // No players to shuffle
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(players.begin(), players.end(), gen);
    
    std::cout << "Player order randomized." << std::endl;
    std::cout << "Play order: ";
    for (size_t i = 0; i < players.size(); ++i) {
        std::cout << players[i]->getName();
        if (i < players.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;
}

/**
 * Startup phase method
 * Commands available - Part 2:
 *
 * loadmap <filename>: Load a map from directory
 * validatemap: Validate loaded map
 * addplayer <playername>: Add players (2-6 players)
 * gamestart: Start game (distribute territories, randomize play order, give 50 initial armies, draw 2 cards)
 */
void GameEngine::startupPhase()
{
    std::cout << "\n=== GAME STARTUP PHASE ===" << std::endl;
    std::cout << "Available user commands:" << std::endl;
    std::cout << "  loadmap <filename> - Load map file from directory" << std::endl;
    std::cout << "  validatemap - Validate loaded map" << std::endl;
    std::cout << "  addplayer <playername> - Add a player (2-6 player limit)" << std::endl;
    std::cout << "  gamestart - Start the game" << std::endl;
    std::cout << "  quit - Exit startup phase\n" << std::endl;
    
    std::string input;
    bool mapLoaded = false;
    bool mapValidated = false;
    
    while (true) {
        std::cout << "\nEnter command: ";
        std::getline(std::cin, input);
        
        if (input.empty()) {
            continue;
        }
        
        std::istringstream iss(input); 
        std::string command;
        iss >> command; // Store first input string into command variable
        
        // Handle loadmap command
        if (command == "loadmap") {
            std::string filename;
            iss >> filename;
            
            if (filename.empty()) {
                // List available map files
                std::cout << "Available map files:" << std::endl;
                std::vector<std::string> mapFiles = getMapFiles();
                for (size_t i = 0; i < mapFiles.size(); ++i) {
                    std::cout << "  " << (i + 1) << ". " << mapFiles[i] << std::endl;
                }
                std::cout << "Usage: loadmap <filename>" << std::endl;
                continue;
            }
            
            // Load the map
            MapLoader loader(filename);
            if (map) {
                delete map;
            }
            map = loader.load();
            
            if (map) {
                mapLoaded = true;
                mapValidated = false; // Reset validation when new map is loaded
                std::cout << "Map loaded successfully: " << filename << std::endl;
                transition("map loaded");
            } else {
                std::cout << "Error: Failed to load map file: " << filename << std::endl;
            }
        }
        // Handle validatemap command
        else if (command == "validatemap") {
            if (!mapLoaded || !map) {
                std::cout << "Error: No map loaded. Please load a map first using 'loadmap <filename>'." << std::endl;
                continue;
            }
            
            if (map->validate()) {
                mapValidated = true;
                std::cout << "Map validation successful! The map is valid." << std::endl;
                transition("map validated");
            } else {
                std::cout << "Map validation failed! The map is invalid." << std::endl;
            }
        }
        // Handle addplayer command
        else if (command == "addplayer") {
            if (!mapValidated) {
                std::cout << "Error: Map must be validated before adding players. Use 'validatemap' first." << std::endl;
                continue;
            }
            
            std::string playerName;
            iss >> playerName;
            
            if (playerName.empty()) {
                std::cout << "Error: Player name required. Usage: addplayer <playername>" << std::endl;
                continue;
            }
            
            // Check if player already exists
            bool playerExists = false;
            for (Player* p : players) {
                if (p && p->getName() == playerName) {
                    playerExists = true;
                    break;
                }
            }
            
            if (playerExists) {
                std::cout << "Error: Player '" << playerName << "' already exists." << std::endl;
                continue;
            }
            
            // Check player limit (2-6 players)
            if (players.size() >= 6) {
                std::cout << "Error: Maximum of 6 players allowed. Cannot add more players." << std::endl;
                continue;
            }
            
            // Add player
            Player* newPlayer = new Player(playerName);
            players.push_back(newPlayer);
            std::cout << "Player '" << playerName << "' added. Total players: " << players.size() << std::endl;
            transition("players added");
        }
        // Handle gamestart command
        else if (command == "gamestart") {
            if (!mapValidated) {
                std::cout << "Error: Map must be validated before starting the game." << std::endl;
                continue;
            }
            
            if (players.size() < 2) {
                std::cout << "Error: At least 2 players required to start the game. Current players: " 
                          << players.size() << std::endl;
                continue;
            }
            
            if (players.size() > 6) {
                std::cout << "Error: Maximum of 6 players allowed. Current players: " << players.size() << std::endl;
                continue;
            }
            
            std::cout << "\n=== STARTING GAME ===" << std::endl;
            
            // a)Fairly distribute all territories to players
            distributeTerritories();
            
            // b)Determine randomly the order of play
            shufflePlayerOrder();
            
            // c)Give 50 initial army units to players (in reinforcement pool)
            for (Player* player : players) {
                if (player) {
                    player->addToReinforcementPool(50);
                }
            }
            
            // d)Let each player draw 2 initial cards from the deck
            // Initialize deck if it doesn't exist
            if (!deck) {
                deck = new Deck();
            }
            
            // Check if enough deck size
            if (deck->size() < players.size() * 2) {
                // Add some cards to deck if needed (create a basic deck)
                for (int i = 0; i < 20; ++i) {
                    CardType types[] = {CardType::Bomb, CardType::Reinforcement, CardType::Blockade, 
                                       CardType::Airlift, CardType::Diplomacy};
                    deck->addCard(new Card(types[i % 5]));
                }
            }
            
            for (Player* player : players) {
                if (player) {
                    for (int i = 0; i < 2; ++i) {
                        Card* card = deck->draw();
                        if (card) {
                            player->getHand()->addCard(card);
                        }
                    }
                    std::cout << "Player " << player->getName() << " draws 2 cards." << std::endl;
                }
            }
            
            // Print player hands after drawing cards
            std::cout << "\n=== Player Hands ===" << std::endl;
            for (Player* player : players) {
                if (player && player->getHand()) {
                    std::cout << "Player " << player->getName() << "'s hand:" << std::endl;
                    std::cout << *player->getHand() << std::endl;
                }
            }
            
            // e)Switch the game to the play phase
            transition("assign reinforcement");
            std::cout << "\n=== GAME STARTED ===" << std::endl;
            std::cout << "Game is now in play phase. Startup phase complete!" << std::endl;
            break;
        }
        // Handle quit command
        else if (command == "quit") {
            std::cout << "Exiting startup phase." << std::endl;
            break;
        }
        else {
            std::cout << "Error: Unknown command '" << command << "'. Type 'quit' to exit." << std::endl;
        }
    }
}

std::vector<Player*>& GameEngine::getPlayers() { return players; }
Map* GameEngine::getMap() { return map; }

/**
 * Runs the main game loop
 */
void GameEngine::mainGameLoop()
{
    int turn = 1;
    // The loop continues until only one player is left
    while (currentState != "win")
    {
        std::cout << "\n--- Turn " << turn++ << " ---" << std::endl;
        if (players.empty()) {
            std::cout << "All players eliminated. No winner." << std::endl;
            break;
        }
        
        // 1. Reinforcement Phase
        reinforcementPhase();
        transition("issue orders"); // State change

        // 2. Issuing Orders Phase
        issueOrdersPhase();
        transition("execute orders"); // State change

        // 3. Orders Execution Phase
        executeOrdersPhase(); // Calls Part 4 Placeholders

        // 4. Check for eliminated players
        removeEliminatedPlayers();
        
        // 5. Check for win condition
        if (checkWinCondition()) {
            transition("win");
            std::cout << "!!! GAME OVER: " << players[0]->getName() << " wins! !!!" << std::endl;
        } else {
            // Main game loop goes back to the reinforcement phase
            transition("assign reinforcement"); // State change
        }
    }
}

/**
 * Implements the reinforcement phase logic
 */
void GameEngine::reinforcementPhase() {
    std::cout << "--- Reinforcement Phase ---" << std::endl;
    for (Player* p : players) {
        // 1. Armies for territories
        int terrBonus = static_cast<int>(std::floor(p->getTerritories()->size() / 3.0));
        
        // 2. Armies for continents
        int contBonus = 0;
        if (map && map->getContinents()) {
            for (Continent* c : *map->getContinents()) {
                if (!c) {
                    continue;
                }

                auto* territories = c->getTerritories();
                if (!territories || territories->empty()) {
                    continue;
                }

                bool ownsAll = true;

                for (Territory* t : *territories) {
                    if (!t || t->getOwner() != p) {
                        ownsAll = false;
                        break;
                    }
                }

                if (ownsAll) {
                    contBonus += c->getControlValue();
                }
            }
        }
        
        // 3. Total (min 3) 
        int total = std::max(3, terrBonus + contBonus);
        p->addToReinforcementPool(total); // "placed in the player's reinforcement pool" 

        std::cout << "Reinforcement -> " << p->getName()
            << " : terr=" << terrBonus
            << " cont=" << contBonus
            << " total=" << total << "\n";
    }
}

/**
 * Implements the order issuing phase logic
 */
void GameEngine::issueOrdersPhase() {
    std::cout << "--- Issuing Orders Phase ---" << std::endl;
    
    // 1. Reset all player "done" flags to false
    for (Player* p : players) {
        p->hasPlayedCardThisRound = false;
        p->setDoneIssuingOrders(false);
    }

    // 2. Loop in round-robin until all players are done
    int playersDone = 0;
    while (playersDone < players.size())
    {
        playersDone = 0; // Reset count each full round
        
        // "called in round-robin fashion across all players"
        for (Player* p : players) {
            if (!p->isDoneIssuingOrders()) {
                p->issueOrder(this); // Player issues one order OR sets flag
            }
            if (p->isDoneIssuingOrders()) { // Check if player set their flag
                playersDone++;
            }
        }
    } // "This phase ends when all players have signified" 
    std::cout << "All players have issued orders." << std::endl;
}

void GameEngine::executeOrdersPhase() {
    std::cout << "--- Executing Orders Phase ---" << std::endl;

    // 1. Execute all DEPLOY orders first (round-robin)
    bool anyExecuted = true;
    while (anyExecuted) {
        anyExecuted = false;
        for (Player* p : players) {
            if (!p || !p->getOrders()) continue;

            auto& vector = p->getOrders()->getOrders();
            for (size_t i = 0; i < vector.size(); ++i) {
                Order* order = vector[i];
                if (order && order->getLabel() == "Deploy") {
                    // executes + validates internally
                    order->execute();
                    delete order;
                    vector.erase(vector.begin() + i);
                    anyExecuted = true;
                    // move to next player
                    break;
                }
            }
        }
    }

    // 2. Execute all remaining orders
    anyExecuted = true;
    while (anyExecuted) {
        anyExecuted = false;
        for (Player* p : players) {
            if (!p || !p->getOrders()) continue;

            auto& vec = p->getOrders()->getOrders();
            for (size_t i = 0; i < vec.size(); ++i) {
                Order* o = vec[i];
                if (o && o->getLabel() != "Deploy") {
                    o->execute();
                    delete o;
                    vec.erase(vec.begin() + i);
                    anyExecuted = true;
                    break;
                }
            }
        }
    }

    // 3. Award cards for successful conquests
    for (Player* p : players) {
        if (p->hasConqueredTerritory()) {
            if (deck) {
                Card* reward = deck->draw();
                if (reward && p->getHand()) {
                    p->getHand()->addCard(reward);
                    std::cout << p->getName() << " received a card for conquering this turn.\n";
                }
            }
            p->setConqueredTerritory(false);
        }

        // Clean up leftover orders
        if (p->getOrders()) {
            p->getOrders()->clear();
        }
    }
}

/**
 * Removes players with no territories
 */
void GameEngine::removeEliminatedPlayers() {
    // Loop with index to allow for safe removal
    for (int i = 0; i < players.size(); ++i) {
        if (players[i]->getTerritories()->empty()) { // "does not control at least one territory"
            std::cout << "Player " << players[i]->getName() << " has been eliminated." << std::endl;
            delete players[i]; // Free memory
            players.erase(players.begin() + i); // "removed from the game"
            --i; // Adjust index after removal
        }
    }
}

/**
 * Checks for a winner
 */
bool GameEngine::checkWinCondition() {
    // "loop shall continue until only one of the players"
    // This is the simplest win condition check
    return players.size() == 1;
}

/**
 * Returns the singleton instance of the Neutral Player.
 * If it does not exist yet, it is dynamically allocated and created.
 * 
 * @return Pointer to the Neutral Player object.
 */
Player* GameEngine::getNeutralPlayer() {
    if (neutralPlayer == nullptr) {
        neutralPlayer = new Player("Neutral");
    }
    return neutralPlayer;
}
/**
 * Replaces the current Neutral Player with a new one.
 * If a neutral player already exists, it is deleted to prevent memory leaks.
 * 
 * @param p Pointer to the new Neutral Player to be assigned.
 */
void GameEngine::setNeutralPlayer(Player* p) {
    if (neutralPlayer != nullptr) {
        delete neutralPlayer;
    }
    neutralPlayer = p;
}

/**
 * Registers a new player in the game engine.
 * The player is added to the internal list only if the pointer is valid.
 * 
 * @param player Pointer to the Player object to be added.
 */
void GameEngine::addPlayer(Player* player) {
    if (player)
        players.push_back(player);
}

void GameEngine::simulateStartup()
{
    std::cout << "\n=== SIMULATED STARTUP PHASE ===" << std::endl;

    // 1. Load a default small map
    MapLoader loader("good_map.txt");
    map = loader.load();

    if (!map) {
        std::cout << "ERROR: simulateStartup() could not load good_map.map" << std::endl;
        return;
    }

    transition("map loaded");

    // 2. Validate map
    if (!map->validate()) {
        std::cout << "ERROR: simulateStartup() map validation failed" << std::endl;
        return;
    }

    transition("map validated");

    // 3. Create 2 players
    players.push_back(new Player("PlayerA"));
    players.push_back(new Player("PlayerB"));

    transition("players added");

    // 4. Create deck
    deck = new Deck();
    CardType types[] = {
        CardType::Bomb, CardType::Reinforcement,
        CardType::Blockade, CardType::Airlift,
        CardType::Diplomacy
    };
    for (int i = 0; i < 20; i++) {
        deck->addCard(new Card(types[i % 5]));
    }

    // 5. Distribute territories
    distributeTerritories();

    // 6. Randomize turn order
    shufflePlayerOrder();

    // 7. Give each player 50 starting armies
    for (Player* p : players) {
        p->addToReinforcementPool(50);
    }

    // 8. Each player draws 2 cards
    for (Player* p : players) {
        for (int i = 0; i < 2; i++) {
            Card* c = deck->draw();
            if (c) p->getHand()->addCard(c);
        }
    }

    // 9. Move to first in-game phase
    transition("assign reinforcement");

    std::cout << "=== Simulated Startup Complete ===" << std::endl;
}


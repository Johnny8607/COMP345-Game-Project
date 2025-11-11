#include "GameEngine.h"
#include "Map.h"
#include "Player.h"
#include "Deck.h"
#include "Cards.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <random>
#include <chrono>

/**
 * Default constructor - initializes game engine to "start" state
 */
GameEngine::GameEngine() : currentState("start"), gameMap(nullptr), players(new std::vector<Player*>()), deck(new Deck())
{
    std::cout << "GameEngine initialized to 'start' state." << std::endl;
}

/**
 * Copy constructor - performs deep copy
 */
GameEngine::GameEngine(const GameEngine &other) : currentState(other.getCurrentState()), gameMap(nullptr), players(nullptr), deck(nullptr)
{
    // Deep copy map if it exists
    if (other.gameMap) {
        gameMap = new Map(*other.gameMap);
    }
    
    // Deep copy players vector
    players = new std::vector<Player*>();
    if (other.players) {
        for (Player* p : *other.players) {
            if (p) {
                players->push_back(new Player(*p));
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
    if (gameMap) {
        delete gameMap;
        gameMap = nullptr;
    }
    
    // Delete all players
    if (players) {
        for (Player* p : *players) {
            if (p) {
                delete p;
            }
        }
        delete players;
        players = nullptr;
    }
    
    // Delete deck
    if (deck) {
        delete deck;
        deck = nullptr;
    }
    
    std::cout << "GameEngine destroyed." << std::endl;
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
        if (gameMap) {
            delete gameMap;
        }
        gameMap = nullptr;
        if (other.gameMap) {
            gameMap = new Map(*other.gameMap);
        }
        
        // Delete existing players
        if (players) {
            for (Player* p : *players) {
                if (p) {
                    delete p;
                }
            }
            delete players;
        }
        players = new std::vector<Player*>();
        if (other.players) {
            for (Player* p : *other.players) {
                if (p) {
                    players->push_back(new Player(*p));
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
    if (!gameMap || !players || players->empty()) {
        std::cout << "Error: Cannot distribute territories - map or players not initialized." << std::endl;
        return;
    }
    
    std::vector<Territory*>* territories = gameMap->getTerritories();
    if (!territories || territories->empty()) {
        std::cout << "Error: No territories to distribute." << std::endl;
        return;
    }
    
    // Shuffle territories for random distribution
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(territories->begin(), territories->end(), gen);
    
    // Distribute territories round-robin style
    size_t playerIndex = 0;
    for (Territory* territory : *territories) {
        if (territory && playerIndex < players->size()) {
            Player* player = (*players)[playerIndex];
            if (player) {
                player->addTerritory(territory);
                territory->setOwner(player);
                playerIndex = (playerIndex + 1) % players->size();
            }
        }
    }
    
    std::cout << "Territories distributed fairly among " << players->size() << " players." << std::endl;
}


// Helper function to shuffle player order randomly
void GameEngine::shufflePlayerOrder()
{
    if (!players || players->empty()) {
        return;
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(players->begin(), players->end(), gen);
    
    std::cout << "Player order randomized." << std::endl;
    std::cout << "Play order: ";
    for (size_t i = 0; i < players->size(); ++i) {
        std::cout << (*players)[i]->getName();
        if (i < players->size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;
}

/**
 * Startup phase method - implements command-based user interaction for game startup
 * Part 2 Requirement: Implements the game startup phase with commands:
 * - loadmap <filename>: Load a map from directory
 * - validatemap: Validate the loaded map
 * - addplayer <playername>: Add players (2-6 players)
 * - gamestart: Start the game (distribute territories, randomize order, give initial armies, draw cards)
 */
void GameEngine::startupPhase()
{
    std::cout << "\n=== GAME STARTUP PHASE ===" << std::endl;
    std::cout << "Available commands:" << std::endl;
    std::cout << "  loadmap <filename> - Load a map file" << std::endl;
    std::cout << "  validatemap - Validate the loaded map" << std::endl;
    std::cout << "  addplayer <playername> - Add a player (2-6 players required)" << std::endl;
    std::cout << "  gamestart - Start the game" << std::endl;
    std::cout << "  quit - Exit startup phase\n" << std::endl;
    
    std::string input;
    bool mapLoaded = false;
    bool mapValidated = false;
    
    while (true) {
        std::cout << "Enter command: ";
        std::getline(std::cin, input);
        
        if (input.empty()) {
            continue;
        }
        
        std::istringstream iss(input);
        std::string command;
        iss >> command;
        
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
            if (gameMap) {
                delete gameMap;
            }
            gameMap = loader.load();
            
            if (gameMap) {
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
            if (!mapLoaded || !gameMap) {
                std::cout << "Error: No map loaded. Please load a map first using 'loadmap <filename>'." << std::endl;
                continue;
            }
            
            if (gameMap->validate()) {
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
            if (players) {
                for (Player* p : *players) {
                    if (p && p->getName() == playerName) {
                        playerExists = true;
                        break;
                    }
                }
            }
            
            if (playerExists) {
                std::cout << "Error: Player '" << playerName << "' already exists." << std::endl;
                continue;
            }
            
            // Check player limit (2-6 players)
            if (players && players->size() >= 6) {
                std::cout << "Error: Maximum of 6 players allowed. Cannot add more players." << std::endl;
                continue;
            }
            
            // Add player
            Player* newPlayer = new Player(playerName);
            players->push_back(newPlayer);
            std::cout << "Player '" << playerName << "' added. Total players: " << players->size() << std::endl;
            transition("players added");
        }
        // Handle gamestart command
        else if (command == "gamestart") {
            if (!mapValidated) {
                std::cout << "Error: Map must be validated before starting the game." << std::endl;
                continue;
            }
            
            if (!players || players->size() < 2) {
                std::cout << "Error: At least 2 players required to start the game. Current players: " 
                          << (players ? players->size() : 0) << std::endl;
                continue;
            }
            
            if (players->size() > 6) {
                std::cout << "Error: Maximum of 6 players allowed. Current players: " << players->size() << std::endl;
                continue;
            }
            
            std::cout << "\n=== STARTING GAME ===" << std::endl;
            
            // a) Fairly distribute all territories to players
            distributeTerritories();
            
            // b) Determine randomly the order of play
            shufflePlayerOrder();
            
            // c) Give 50 initial army units to players (in reinforcement pool)
            for (Player* player : *players) {
                if (player) {
                    player->setReinforcementPool(50);
                    std::cout << "Player " << player->getName() << " receives 50 initial army units." << std::endl;
                }
            }
            
            // d) Let each player draw 2 initial cards from the deck
            // First, ensure deck has enough cards
            if (deck->size() < players->size() * 2) {
                // Add some cards to deck if needed (create a basic deck)
                for (int i = 0; i < 20; ++i) {
                    CardType types[] = {CardType::Bomb, CardType::Reinforcement, CardType::Blockade, 
                                       CardType::Airlift, CardType::Diplomacy};
                    deck->addCard(new Card(types[i % 5]));
                }
            }
            
            for (Player* player : *players) {
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
            
            // e) Switch the game to the play phase
            transition("assign reinforcement");
            std::cout << "\n=== GAME STARTED ===" << std::endl;
            std::cout << "Game is now in play phase. Startup phase complete." << std::endl;
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

/**
 * Main game loop - placeholder for future implementation
 */
void GameEngine::mainGameLoop()
{
    std::cout << "Main game loop - not yet implemented." << std::endl;
}

/**
 * Reinforcement phase - placeholder for future implementation
 */
void GameEngine::reinforcementPhase()
{
    std::cout << "Reinforcement phase - not yet implemented." << std::endl;
}

/**
 * Issue orders phase - placeholder for future implementation
 */
void GameEngine::issueOrdersPhase()
{
    std::cout << "Issue orders phase - not yet implemented." << std::endl;
}

/**
 * Execute orders phase - placeholder for future implementation
 */
void GameEngine::executeOrdersPhase()
{
    std::cout << "Execute orders phase - not yet implemented." << std::endl;
}
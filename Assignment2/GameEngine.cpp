#include "GameEngine.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <random>

#include "Map.h"
#include "Player.h"
#include "Cards.h"
#include "Deck.h"
#include "Hand.h"
#include "Orders.h"

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
GameEngine::GameEngine(const GameEngine &other) : currentState(other.getCurrentState())
{
    // A full deep copy would be required for A2 once all part are implemented
    std::cout << "GameEngine copied." << std::endl;
}

/**
 * Destructor - cleans up resources
 */
GameEngine::~GameEngine()
{
    // --- NEW FOR A2 ---
    delete map;
    map = nullptr;
    delete deck;
    deck = nullptr;
    for (Player* p : players) {
        delete p;
    }
    // --- END NEW ---
    std::cout << "GameEngine destroyed." << std::endl;
}

/**
 * Assignment operator - assigns one GameEngine to another
 */
GameEngine &GameEngine::operator=(const GameEngine &other)
{
    if (this != &other)
    {
        // A full deep copy would be required for A2 once all parts are implemented
        currentState = other.getCurrentState();
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

// --- NEW A2 GETTERS ---
std::vector<Player*>& GameEngine::getPlayers() { return players; }
Map* GameEngine::getMap() { return map; }
// --- END NEW ---

/**
 * == A2 PART 2: PLACEHOLDER ==
 * This is a stub function to set up the game state
 * so that the Main Game Loop (Part 3) can be tested.
 * It manually creates players, a map, and distributes
 * territories and cards.
 * 
 * Will replace with actual implementation in A2 Part 4.
 */
void GameEngine::startupPhase() {
    transition("startup");
    std::cout << "--- PLACEHOLDER: Executing STUB Startup Phase ---" << std::endl;

    // 1. Create Map
    map = new Map();
    Continent* c = new Continent("Stub Continent", 5);
    map->addContinent(c);
    Territory* t1 = new Territory("TerritoryA");
    Territory* t2 = new Territory("TerritoryB");
    Territory* t3 = new Territory("TerritoryC");
    map->addTerritory(t1); map->addTerritory(t2); map->addTerritory(t3);
    c->addTerritory(t1); c->addTerritory(t2); c->addTerritory(t3);
    std::cout << "Stub map created." << std::endl;

    // 2. Create Players
    players.push_back(new Player("PlayerA"));
    players.push_back(new Player("PlayerB"));
    std::cout << "Stub players created." << std::endl;

    // 3. Create Deck
    deck = new Deck();
    deck->addCard(new Card(CardType::Bomb));
    deck->addCard(new Card(CardType::Airlift));
    std::cout << "Stub deck created." << std::endl;

    // 4. "gamestart" command
    // a) Distribute territories
    players[0]->addTerritory(t1); t1->setOwner(players[0]);
    players[1]->addTerritory(t2); t2->setOwner(players[1]);
    players[0]->addTerritory(t3); t3->setOwner(players[0]);
    std::cout << "Stub territories distributed." << std::endl;
    
    // b) Determine order of play
    std::cout << "Player order is set." << std::endl;

    // c) Give 50 initial armies
    for (Player* p : players) p->addToReinforcementPool(50);

    // d) Give 2 initial cards
    players[0]->getHand()->addCard(deck->draw());
    players[0]->getHand()->addCard(deck->draw());
    // (Player 2 gets no cards, deck is now empty)
    std::cout << "Players given 2 cards (stub)." << std::endl;

    // e) Switch to play phase
    transition("assign reinforcement");
    std::cout << "--- PLACEHOLDER: Startup Complete ---" << std::endl;
}


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
        
        // 5. Check for win condition [cite: 120]
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
        // 1. Armies for territories (num territories / 3)
        int terrBonus = std::floor(p->getTerritories()->size() / 3);
        
        // 2. Armies for continents
        int contBonus = 0;
        // --- PLACEHOLDER ---
        // A real implementation would check if the player
        // owns all territories in each continent and add the bonus value.
        // for (Continent* c : *map->getContinents()) { ... }
        
        // 3. Total (min 3) 
        int total = std::max(3, terrBonus + contBonus);
        p->addToReinforcementPool(total); // "placed in the player's reinforcement pool" 
    }
}

/**
 * Implements the order issuing phase logic
 */
void GameEngine::issueOrdersPhase() {
    std::cout << "--- Issuing Orders Phase ---" << std::endl;
    
    // 1. Reset all player "done" flags to false
    for (Player* p : players) {
        p->setDoneIssuingOrders(false);
    }

    // 2. Loop in round-robin until all players are done
    int playersDone = 0;
    while (playersDone < players.size())
    {
        playersDone = 0; // Reset count each full round
        
        // "called in round-robin fashion across all players" [cite: 114]
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

/**
 * == A2 PART 3: HELPER IMPLEMENTATION ==
 * Implements the order *execution loop* logic
 * This calls the STUB orders (Part 4 placeholder).
 * 
 * Will be replaced with actual implementation in A2 Part 4.
 */
void GameEngine::executeOrdersPhase() {
    std::cout << "--- Executing Orders Phase ---" << std::endl;

    // "The game engine should execute all the deploy orders before..." 
    std::cout << "--- Executing DEPLOYS (Placeholders) ---" << std::endl;
    for (Player* p : players) {
        OrdersList* list = p->getOrders();
        for (Order* o : list->getOrders()) {
            if (o->getLabel() == "Deploy") { // Check order type
                o->execute(); // Calls A1 stub
            }
        }
    }
    
    // "...it executes any other kind of order." 
    std::cout << "--- Executing OTHER ORDERS (Placeholders) ---" << std::endl;
    for (Player* p : players) {
        for (Order* o : p->getOrders()->getOrders()) {
            if (o->getLabel() != "Deploy") { // Check order type
                o->execute(); // Calls A1 stub
            }
        }
    }

    // 3. Award cards (stub)
    // "A player receives a card... if they successfully conquered..." 
    for (Player* p : players) {
        // --- PLACEHOLDER ---
        // A real Part 4 implementation would set this flag
        // if (p->hasConqueredTerritory()) {
        //    std::cout << p->getName() << " gets a card (stub)." << std::endl;
        //    p->getHand()->addCard(deck->draw());
        //    p->setConqueredTerritory(false); // Reset flag
        // }

        // Clear all orders from this turn
        p->getOrders()->clear(); // Uses new method from Step 2
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
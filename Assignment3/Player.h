// Header guards
#ifndef PLAYER_H
#define PLAYER_H

// STL Standard Template Library headers
#include <iostream>
#include <string>
#include <vector>

// Custom headers from project needed for Player class
#include "Cards.h"
#include "Map.h"
#include "Orders.h"
#include "Hand.h"
#include "Deck.h"
#include "LoggingObserver.h" // For ILoggable and Subject

// Forward declarations
class Territory;
class Hand;
class OrdersList;
class Deck;
class GameEngine;
class PlayerStrategy; // NEW for A3

// Player class following the Rule of Three for careful memory management
class Player : public ILoggable, public Subject { // Inherits for Observer Pattern
public:
    bool hasPlayedCardThisRound = false;

    // Constructors
    Player(const std::string& name); // Set player name
    Player(const Player& other); // Copy constructor
    Player& operator=(const Player& other); // Copy assignment operator
    ~Player(); // Destructor (cleans up dynamic members)

    // Accessor getter methods
    std::string getName() const; // Get player name
    std::vector<Territory*>* getTerritories() const; // Pointer to territories vector for player
    Hand* getHand() const; // Pointer to player's hand cards
    OrdersList* getOrders() const; // Pointer to player's order list
    int getReinforcementPool() const; // @return The number of armies in the reinforcement pool
    bool isDoneIssuingOrders() const; // @return True if player is done issuing orders
    bool hasConqueredTerritory() const; // @return True if player conquered a territory this turn
    bool isCeasefireWith(Player* other) const; // Checks if a ceasefire exists
    bool hasAdjacentTerritory(Territory* target) const; // Checks adjacency for attack validation

    // Mutator setter methods
    void addOrder(Order* order); // Function to add a new order to player's order list
    void addTerritory(Territory* territory); // Add territory pointer
    void addToReinforcementPool(int armies);
    void setReinforcementPool(int value); 
    void setDoneIssuingOrders(bool done);
    void setConqueredTerritory(bool conquered);
    
    void addCeasefire(Player* other);
    void clearCeasefire();
    void playCard(Deck *deck);

    // Strategy Management
    void setStrategy(PlayerStrategy* newStrategy);
    PlayerStrategy* getStrategy() const;
    
    // Core Game Methods (Delegated to Strategy)
    // NOTE: These match the definitions used in Player.cpp and are NOT marked 'override'
    std::vector<Territory*> toDefend(); 
    std::vector<Territory*> toAttack(); 
    void issueOrder(GameEngine* game);

    // Obsolete Method (kept for old driver compatibility, as seen in Player.cpp)
    void issueOrder(const std::string& type);
    
    // Stream insertion for help printing Player object; set as friend
    friend std::ostream& operator<<(std::ostream& os, const Player& player);

    // Observer Pattern Implementation
    std::string stringToLog() const override; 


private:
    // Data members (Matching Player.cpp definitions)
    std::string name;                       // Player name (NOT a pointer, matching Player.cpp)
    std::vector<Territory*>* territories;   // Pointer to vector pointers territories of player
    Hand* hand;                             // Player's hand of cards
    OrdersList* orders;                     // Player's orders list
    int reinforcementPool;                  // Reinforcement pool (NOT a pointer, matching Player.cpp)
    bool DoneIssuingOrders;                 // Flag indicating if player is done issuing orders
    bool ConqueredTerritoryThisTurn;        // Flag indicating if player conquered a territory this turn
    std::vector<Player*> ceasefirePlayers;  // Used to track players this player is in ceasefire with
    PlayerStrategy* strategy;               // Pointer to the PlayerStrategy used by this player
    
    // To delete safely all player object members using dynamic memory
    void clearData();

    // Copy over data from another player
    void copyFrom(const Player& other);
};
#endif // PLAYER_H
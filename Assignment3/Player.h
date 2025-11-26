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

// Forward declarations
class Territory;
class Hand;
class OrdersList;
class Deck;
class GameEngine;
class PlayerStrategy; // NEW for A3

// Player class following the Rule of Three for careful memory management
class Player { 

public:
    bool hasPlayedCardThisRound = false;

    // Constructors
    Player(const std::string& name); // Set player name
    Player(const Player& other); // Copy constructor for copying and creating from an existing player
    Player& operator=(const Player& other); // Copy assignment operator for deep copying over an existing player
    ~Player(); // Destructor for Player class to clean up allocated memory

    // Accessor getter methods
    std::string getName() const; // Get player name
    std::vector<Territory*>* getTerritories() const; // Pointer to territories vector for player
    Hand* getHand() const; // Pointer to player's hand cards
    void addOrder(Order* order); // Function to add a new order to player's order list
    OrdersList* getOrders() const; // Pointer to player's orders list
    void playCard(Deck* deck);

    // NEW for A3: Assigns a strategy to this Player
    void setStrategy(PlayerStrategy* newStrategy);
    PlayerStrategy* getStrategy() const;

    // Add territory pointer to player's territory list
    void addTerritory(Territory* territory);

    // Returns territories to defend; arbitrary implementation for assignment1
    std::vector<Territory*> toDefend() const;  // arbitrary list of territories to defend

    // Returns territories to attack; arbitrary implmentation for assignment1
    std::vector<Territory*> toAttack() const;  // arbitrary list of territories to attack

    // Order creation and add to OrdersList
    void issueOrder(const std::string& orderType);

    // The main decision-making method for a player (overloaded version)
    void issueOrder(GameEngine *game);

    // Getters
    int getReinforcementPool() const;
    bool isDoneIssuingOrders() const;
    bool hasConqueredTerritory() const;
    bool isCeasefireWith(Player* other) const;
    bool hasAdjacentTerritory(Territory* target) const;

    // Setters
    void addToReinforcementPool(int armies);
    void setDoneIssuingOrders(bool done);
    void setConqueredTerritory(bool conquered);
    
    void setReinforcementPool(int value); 
    void addCeasefire(Player* other);
    void clearCeasefire();

    // Stream insertion for help printing Player object; set as friend to access class private members
    friend std::ostream& operator<<(std::ostream& os, const Player& player);


private:

    // Data members for user-defined Player class set as pointers for the assignment
    std::string name;                       // Player name
    std::vector<Territory*>* territories;   // Pointer to vector pointers territories of player
    Hand* hand;                             // Player's hand of cards
    OrdersList* orders;                     // Player's orders list
    int reinforcementPool;                  // Reinforcement pool (army units available for deployment)
    bool DoneIssuingOrders;                 // Flag indicating if player is done issuing orders
    bool ConqueredTerritoryThisTurn;        // Flag indicating if player conquered a territory this turn
    std::vector<Player*> ceasefirePlayers;  // Used to track players this player is in ceasefire with
    PlayerStrategy* strategy;               //NEW dor A3: Pointer to the PlayerStrategy used by this player
    // To delete safely all player object members using dynamic memory
    void clearData();

    // Copy over data from another player
    void copyFrom(const Player& other);
};
#endif // PLAYER_H

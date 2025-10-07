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

// Player class following the Rule of Three for careful memory management
class Player { 

public:

    // Constructors
    Player(const std::string& name); // Set player name
    Player(const Player& other); // Copy constructor for copying and creating from an existing player
    Player& operator=(const Player& other); // Copy assignment operator for deep copying over an existing player
    ~Player(); // Destructor for Player class to clean up allocated memory

    // Accessor getter methods
    std::string getName() const; // Get player name
    std::vector<Territory*>* getTerritories() const; // Pointer to territories vector for player
    Hand* getHand() const; // Pointer to player's hand cards
    OrdersList* getOrders() const; // Pointer to player's orders list

    // Add territory pointer to player's territory list
    void addTerritory(Territory* territory);

    // Returns territories to defend; arbitrary implementation for assignment1
    std::vector<Territory*> toDefend() const;  // arbitrary list of territories to defend

    // Returns territories to attack; arbitrary implmentation for assignment1
    std::vector<Territory*> toAttack() const;  // arbitrary list of territories to attack

    // Order creation and add to OrdersList
    void issueOrder(const std::string& orderType);

    // Stream insertion for help printing Player object; set as friend to access class private members
    friend std::ostream& operator<<(std::ostream& os, const Player& player);


private:

    // Data members for user-defined Player class set as pointers for the assignment
    std::string name;                       // Player name
    std::vector<Territory*>* territories;   // Pointer to vector pointers territories of player
    Hand* hand;                             // Player's hand of cards
    OrdersList* orders;                     // Player's orders list

    // To delete safely all player object members using dynamic memory
    void clearData();

    // Copy over data from another player
    void copyFrom(const Player& other);
};
#endif // PLAYER_H
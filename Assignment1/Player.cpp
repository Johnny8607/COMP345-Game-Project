#include "Player.h"
#include "Map.h" // Territory, map
#include "Hand.h" // Hand, cards
#include "Orders.h" // Orders, OrdersList
#include <algorithm> // Operations, vector

// Constructor using 'new' dynamic memory allocation for name, territories, hand, orders
Player::Player(const std::string& name)
    : name(name), territories(new std::vector<Territory*>()), hand(new Hand()), orders(new OrdersList()) {}

// Copy constructor for copying and creating from an existing player
Player::Player(const Player& other)
    : name(other.name), territories(nullptr), hand(nullptr), orders(nullptr) {
    copyFrom(other);
}

// Copy assignment operator for deep copying over an existing player
Player& Player::operator=(const Player& other) {
    if (this != &other) {
        clearData();
        name = other.name;
        copyFrom(other);
    }
    return *this;
}

// Destructor for Player class to clean up allocated memory
Player::~Player() {
    clearData();
}

// To delete safely all player object members using dynamic memory
void Player::clearData() {
    if (territories) {
        delete territories; 
        territories = nullptr; 
    }

    if (hand) { 
        delete hand; 
        hand = nullptr; 
    }

    if (orders) {
        delete orders; 
        orders = nullptr; 
    }
}

// Make new dynamic members copying from another player
void Player::copyFrom(const Player& other) {
    // Copy over pointers vector
    territories = new std::vector<Territory*>(*other.territories);
    // Copy over hand
    hand = new Hand(*other.hand);
    // Fresh blank orders list
    orders = new OrdersList();
}

// Getters definitions
std::string Player::getName() const {
    return name;
}

std::vector<Territory*>* Player::getTerritories() const {
    return territories;
}

Hand* Player::getHand() const { 
    return hand;
}

OrdersList* Player::getOrders() const {
    return orders;
}

// Add territory pointer if not null to vector pointing to player territories
void Player::addTerritory(Territory* territory) {
    if (!territory) return;
    territories->push_back(territory);
}

// Return up to three first territories to defend from player list
std::vector<Territory*> Player::toDefend() const {
    std::vector<Territory*> selection;

    // Check territories exist
    if (!territories || territories->empty()) return selection;

    // Select up to 3 territories from the start of territory list
    for (size_t i = 0; i < territories->size() && i < 3; ++i) {
        selection.push_back((*territories)[i]);
    }
    return selection;
}

// Return up to three territories to attack from player list after the first three
std::vector<Territory*> Player::toAttack() const {
    std::vector<Territory*> selection;

    // Check territories exist
    if (!territories || territories->empty()) return selection;

    // Select up to 3 territories from the end of defending territory list
    for (size_t i = 3; i < territories->size() && selection.size() < 3; ++i) {
        selection.push_back((*territories)[i]);
    }
    return selection;
}

// Create and add order to list if possible
void Player::issueOrder(const std::string& orderType) {
    OrderFactory factory;
    Order* order = factory.createOrder(orderType);
    if (order) {
        orders->addOrder(order);
    }
}

// Stream insertion for help printing Player object in specified format
std::ostream& operator<<(std::ostream& os, const Player& player) {
    os << "Player: " << player.name;
    return os; // Returns Player: player name
}
#include "Player.h"
#include "Map.h" // Territory, map
#include "Hand.h" // Hand, cards
#include "Orders.h" // Orders, OrdersList
#include "GameEngine.h"
#include <algorithm> // Operations, vector
#include <random>

// Constructor using 'new' dynamic memory allocation for name, territories, hand, orders
Player::Player(const std::string& name)
    : name(name), territories(new std::vector<Territory*>()), hand(new Hand()), orders(new OrdersList()),
    // --- NEW FOR A2 PART 3 ---
      reinforcementPool(0),
      DoneIssuingOrders(false),
      ConqueredTerritoryThisTurn(false)
      // --- END NEW ---
    {}

// Copy constructor for copying and creating from an existing player
Player::Player(const Player& other)
    : name(other.name), territories(nullptr), hand(nullptr), orders(nullptr),
    // --- NEW FOR A2 PART 3 ---
      reinforcementPool(other.reinforcementPool),
      DoneIssuingOrders(other.DoneIssuingOrders),
      ConqueredTerritoryThisTurn(other.ConqueredTerritoryThisTurn)
    // --- END NEW ---
    {
    copyFrom(other);
}

// Copy assignment operator for deep copying over an existing player
Player& Player::operator=(const Player& other) {
    if (this != &other) {
        clearData();
        name = other.name;
        copyFrom(other);
    }
    // --- NEW FOR A2 PART 3 ---
        reinforcementPool = other.reinforcementPool;
        DoneIssuingOrders = other.DoneIssuingOrders;
        ConqueredTerritoryThisTurn = other.ConqueredTerritoryThisTurn;
    // --- END NEW ---
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

void Player::addOrder(Order *order) {
    // do nothing if order is null
    if (!order) {
        return;
    } 
    orders->addOrder(order);
}

OrdersList* Player::getOrders() const {
    return orders;
}

void Player::playCard(Deck *deck) {
    auto& cards = hand->getAllCards();

    if (cards.empty()) {
        return;
    }

    // random device generator used to get a random number
    std::random_device random_device;
    std::mt19937 gen(random_device());
    std::uniform_int_distribution<> dis(0, cards.size() - 1);
    int randomNumber = dis(gen);

    // use random number to play a random card
    Card* card = cards.at(randomNumber);
    card->play(this);
    hand->removeCard(card);
    deck->addCard(card);
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

// --- NEW FOR A2 PART 3 ---

/**
 * @return The number of armies in the player's reinforcement pool.
 */
int Player::getReinforcementPool() const {
    return reinforcementPool;
}

/**
 * @return True if the player is done issuing orders for the turn.
 */
bool Player::isDoneIssuingOrders() const {
    return DoneIssuingOrders;
}

/**
 * @return True if the player conquered a territory this turn.
 */
bool Player::hasConqueredTerritory() const {
    return ConqueredTerritoryThisTurn;
}

/**
 * Adds armies to the player's reinforcement pool.
 * @param armies The number of armies to add.
 */
void Player::addToReinforcementPool(int armies) {
    reinforcementPool += armies;
    std::cout << "Player " << name << " receives " << armies << " reinforcements. (Total: " << reinforcementPool << ")" << std::endl;
}

/**
 * Sets the player's "done issuing orders" flag.
 * @param done The new status.
 */
void Player::setDoneIssuingOrders(bool done) {
    DoneIssuingOrders = done;
    if(done) {
        std::cout << "Player " << name << " is done issuing orders." << std::endl;
    }
}

/**
 * Sets the player's "conquered territory" flag.
 * @param conquered The new status.
 */
void Player::setConqueredTerritory(bool conquered) {
    ConqueredTerritoryThisTurn = conquered;
}

/**
 * @brief The main decision-making method for a player.
 * This implements the logic described in the "Orders Issuing phase".
 */
void Player::issueOrder(GameEngine* game) {
    
    // "As long as the player has army units in their reinforcement pool...
    // it will issue a deploy order and no other order."
    if (reinforcementPool > 0) {
        // --- PLACEHOLDER ---
        // A real implementation would use toDefend() to pick a territory.
        // This stub just deploys 3 armies to the first available territory.
        
        int armiesToDeploy = std::min(3, reinforcementPool); // Deploy 3 or all remaining
        reinforcementPool -= armiesToDeploy;
        
        std::cout << "Player " << name << " issuing DEPLOY of " << armiesToDeploy << "(hard coded) armies." << std::endl;
        issueOrder("deploy"); // Use A1 factory to create a stub "deploy" order
        return; // One order issued, done for this round-robin tick.
    }

    // "Once it has deployed all... it can proceed with other kinds of orders." [cite: 128]
    // --- PLACEHOLDER ---
    // A real implementation would use toAttack() or playCard().
    // This stub just issues one "advance" order and then finishes.
    
    // Check if we have already issued our one "advance" order
    if (orders->getOrders().size() == 0 || orders->getOrders().back()->getLabel() == "Deploy") {
        std::cout << "Player " << name << " issuing ADVANCE (stub)." << std::endl;
        issueOrder("advance"); // Use A1 factory to create a stub "advance" order
        return; // One order issued.
    }

    // If we get here, we have deployed and issued our one advance order.
    // We signify that we are done for the turn.
    setDoneIssuingOrders(true);
}

// --- END NEW ---

// Stream insertion for help printing Player object in specified format
std::ostream& operator<<(std::ostream& os, const Player& player) {
    os << "Player: " << player.name;
    return os; // Returns Player: player name
}
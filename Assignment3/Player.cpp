#include "Player.h"
#include "Map.h" // Territory, map
#include "Hand.h" // Hand, cards
#include "Orders.h" // Orders, OrdersList
#include "GameEngine.h"
#include "Cards.h"
#include "PlayerStrategies.h"   //NEW for A3
#include <algorithm> // Operations, vector
#include <random>

// Constructor using 'new' dynamic memory allocation for name, territories, hand, orders
Player::Player(const std::string& name)
    : name(name), territories(new std::vector<Territory*>()), hand(new Hand()), orders(new OrdersList()),
      reinforcementPool(0),
      DoneIssuingOrders(false),
      ConqueredTerritoryThisTurn(false),
      strategy(nullptr)  //NEW for A3
    {
     //NEw for A3: Per assignment, Player must always have a strategy
      setStrategy(new HumanPlayerStrategy());
    }

// Copy constructor for copying and creating from an existing player
Player::Player(const Player& other)
    : name(other.name), territories(nullptr), hand(nullptr), orders(nullptr),
      reinforcementPool(other.reinforcementPool),
      DoneIssuingOrders(other.DoneIssuingOrders),
      ConqueredTerritoryThisTurn(other.ConqueredTerritoryThisTurn),
      strategy(nullptr)//NEW for A3
    {
    copyFrom(other);
        if (other.strategy) { //NEW for A3
        strategy = other.strategy->clone();
        if (strategy) strategy->setPlayer(this);
    }
}

// Copy assignment operator for deep copying over an existing player
Player& Player::operator=(const Player& other) {
    if (this != &other) {
        clearData();
        name = other.name;
        copyFrom(other);
        
        // NEW for A3: Copy strategy
        if (other.strategy) {
            strategy = other.strategy->clone();
            if (strategy) strategy->setPlayer(this);
        } else {
            strategy = nullptr;
        }
    
        reinforcementPool = other.reinforcementPool;
        DoneIssuingOrders = other.DoneIssuingOrders;
        ConqueredTerritoryThisTurn = other.ConqueredTerritoryThisTurn;
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

    if (strategy) {  //NEW for A3
        delete strategy;
        strategy = nullptr;
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

// NEW for A3
// Strategy methods
void Player::setStrategy(PlayerStrategy* newStrategy) {
    if (strategy) {
        delete strategy;
        strategy = nullptr;
    }
    strategy = newStrategy;
    if (strategy) strategy->setPlayer(this);
}

PlayerStrategy* Player::getStrategy() const {
    return strategy;
}


// Returns true if this player currently has a ceasefire agreement with `other`.
bool Player::isCeasefireWith(Player* other) const {
    for (auto* p : ceasefirePlayers)
        if (p == other) return true;
    return false;
}

// Adds a ceasefire agreement with another player, if not already present.
// Prevents duplicate entries in the ceasefire list.
void Player::addCeasefire(Player* other) {
    if (!isCeasefireWith(other))
        ceasefirePlayers.push_back(other);
}

// Clears all active ceasefire agreements.
void Player::clearCeasefire() {
    ceasefirePlayers.clear();
}

// Checks if the target territory is adjacent to any territory owned by this player.
// Returns true if at least one owned territory is adjacent to the target.
bool Player::hasAdjacentTerritory(Territory* target) const {
    for (auto* t : *territories) {
        if (t->isAdjacentTo(target))
            return true;
    }
    return false;
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
    territory->setOwner(this); //New for A3
}

// New for A3
// Return up to three first territories to defend from player list
std::vector<Territory*> Player::toDefend() const {
     if (strategy) return strategy->toDefend();
    return {};
}

// New for A3
// Return up to three territories to attack from player list after the first three
std::vector<Territory*> Player::toAttack() const {
   if (strategy) return strategy->toAttack();
    return {};
}

void Player::issueOrder(const std::string& type)
{
    // ONLY HERE TO MAKE PlayerDriver COMPILE
    if (type == "deploy") {
        cout << name << " issues DEPLOY (stub)" << endl;
        orders->addOrder(new Deploy()); // stub only
    }
    else if (type == "advance") {
        cout << name << " issues ADVANCE (stub)" << endl;
        orders->addOrder(new Advance());
    }
}


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
 * Set the number of armies available in the player's reinforcement pool.
 * @param value The new number of reinforcement armies for the player
 *
*/
void Player::setReinforcementPool(int value) {
    reinforcementPool = value;
}

/**
 * NEW for A3
 * @brief Player class does not implement behavior
 * simply delegates to the PlayerStrategy
 */
void Player::issueOrder(GameEngine* engine)
{
   if (strategy)
        strategy->issueOrder(engine);
}


// Stream insertion for help printing Player object in specified format
std::ostream& operator<<(std::ostream& os, const Player& player) {
    os << "Player: " << player.name;
    return os; // Returns Player: player name
}

#include "Orders.h"
#include "Player.h"
#include "Map.h"
#include "GameEngine.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

// Static order labels
const string Deploy::label   = "Deploy";
const string Advance::label  = "Advance";
const string Bomb::label     = "Bomb";
const string Blockade::label = "Blockade";
const string Airlift::label  = "Airlift";
const string Negotiate::label = "Negotiate";


//virtual desreuctor
Order::~Order() = default;

/**
 * @param out Reference to an output stream (e.g., std::cout).
 * @param order Reference to the Order object to print.
 * @return The same output stream, for chaining.
 */
ostream& operator<<(ostream& out, const Order& order) {
    return order.print(out);
}


//Default constructor 
OrdersList::OrdersList() = default;
OrdersList::OrdersList(const OrdersList& other) {
    for (auto* o : other.orders_) {
        orders_.push_back(o->clone());
    }
}
// Deep copy assignment operator
OrdersList& OrdersList::operator=(const OrdersList& other) {
    if (this != &other) {
        for (auto* o : orders_) delete o;
        orders_.clear();
        for (auto* o : other.orders_) {
            orders_.push_back(o->clone());
        }
    }
    return *this;
}
OrdersList::~OrdersList() {
    for (auto* o : orders_){
        delete o;
    }
    orders_.clear();
}
void OrdersList::clear() {
    for (auto* o : orders_) {
        delete o;
    }
    orders_.clear();
}
/**
 * Adds a new Order pointer to the OrdersList.
 * @param order Pointer to the dynamically allocated Order object.
 */
void OrdersList::addOrder(Order* order) {
    if (!order) {
        cout << "Opps! Invalid order!\n";
        return;
    }
    orders_.push_back(order);
     cout << "\n=========== ADDING ORDERS ===========\n";
    cout << "No.  | Order\n";
    cout << "----------------\n";

    // Print each order in the table
    for (size_t i = 0; i < orders_.size(); ++i) {
        cout << (i + 1) << "    | " << orders_[i]->getLabel() << "\n";
    }

    cout << "----------------\n";
}

/**
 * Removes an Order from the list by its position.
 * @param index 1-based index of the order to remove (1 = first order).
 * Deletes the order from memory and removes it from the container.
 */
void OrdersList::remove(int index) {
    if (orders_.empty()) {
        cout << "No orders to remove.\n";
        return;
    }
    if (index < 1 || index > orders_.size()) {
        cout << "Opps! Invalid position!\n";
        return;
    }

    delete orders_[index - 1];
    orders_.erase(orders_.begin() + (index - 1));
    cout << "Order at position " << index << " removed.\n";
    cout << *this << endl;
}

/**
 * Moves an Order from one position to another within the list.
 * @param from 1-based starting position of the order to move.
 * @param to 1-based target position where the order should be placed.
 */
void OrdersList::move(int from, int to) {
    int n = static_cast<int>(orders_.size());
    if (n==0||n < 2) { cout << "Not enough orders to move.\n"; return; }
    if (from < 1 || to < 1 || from > n || to > n) { cout << "Invalid move positions.\n"; return; }
    if (from == to) { cout << "Nothing to move.\n"; return; }

    std::swap(orders_[from - 1], orders_[to - 1]);
    cout << "Moved order from " << from << " to " << to << ".\n";
    cout << *this << endl;
}

//Execute all orders in the list sequentially, then deletes and clears them fro memory.
void OrdersList::executeOrders() {
    if (orders_.empty()) {
        cout << "No orders to execute.\n";
        return;
    }

    for (auto* o : orders_)
        o->execute();

    for (auto* o : orders_)
        delete o;

    orders_.clear();
    cout << "All orders executed and cleared.\n";
}

vector<Order*>& OrdersList::getOrders() {
    return orders_;
}

/**
 * Stream insertion operator for printing the current list of orders.
 * @param out Reference to output stream
 * @param ol Constant reference to the OrdersList to print.
 * @return The same output stream for chaining.
 */
ostream& operator<<(ostream& out, const OrdersList& ol) {
    if (ol.orders_.empty()) {
        out << "(empty)\n";
        return out;
    }

    out << "No.  | Order\n";
    out << "---------------\n";
    for (size_t i = 0; i < ol.orders_.size(); ++i)
        out << (i + 1) << "    | " << ol.orders_[i]->getLabel() << "\n";

    return out;
}


// =====================  ORDER SUBCLASSES  =====================

/**
 * Prints the name of this specific Order type.
 * @param out Reference to an output stream.
 * @return 
 */
ostream& Deploy::print(ostream& out) const   { return out << "Deploy Order"; }
ostream& Advance::print(ostream& out) const  { return out << "Advance Order"; }
ostream& Bomb::print(ostream& out) const     { return out << "Bomb Order"; }
ostream& Blockade::print(ostream& out) const { return out << "Blockade Order"; }
ostream& Airlift::print(ostream& out) const  { return out << "Airlift Order"; }
ostream& Negotiate::print(ostream& out) const{ return out << "Negotiate Order"; }


/**
 * Return the name of the Order.
 * @return The order’s name.
 */
string Deploy::getLabel()   const { return label; }
string Advance::getLabel()  const { return label; }
string Bomb::getLabel()     const { return label; }
string Blockade::getLabel() const { return label; }
string Airlift::getLabel()  const { return label; }
string Negotiate::getLabel()const { return label; }

/**
 * Checks if the Order is valid.
 * @return True or false
 */

/**
 * - Validates Deploy order.
 * - Target must exist and belong to issuing player.
 * - Armies must be positive and within reinforcement pool.
 * @return true if valid, false otherwise.
 */
bool Deploy::validate()   const { 
    cout << "Validating Deploy...\n"; 
    if (!target || !player) return false;
    if (target->getOwner() != player) { 
        cout << "Invalid! you do not own this territory.\n"; 
        return false; }
    if (armies <= 0) { 
        cout << "Invalid! You must deploy positive number armies.\n"; 
        return false; }
    if (armies > player->getReinforcementPool()) { 
        cout << "Invalid! You do not have enough armies in pool.\n"; 
        return false; } 
        return true; }

/**
 * - Validates Advance order.
 * - Source must belong to player.
 * - Target must be adjacent.
 * - Army count must be valid.
 * - Cannot attack if ceasefire exists.
 * @return true if valid, false otherwise.
 */
bool Advance::validate()  const { 
    cout << "Validating Advance...\n"; 
    if (!player || !source || !target) return false;
    if (source->getOwner() != player) { 
        cout << "Source do not belong to you.\n"; 
        return false; }
    if (!source->isAdjacentTo(target)) { 
        cout << "Target is not adjacent.\n"; 
        return false; }
    if (armies <= 0 || armies > source->getArmies()) { 
        cout << "Invalid army number!\n"; 
        return false; }
    if (player->isCeasefireWith(target->getOwner())) { 
        cout << "Ceasefire active, cannot attack.\n";
         return false; 
        }return true; }
/**
 * - Validates Bomb order.
 * - Target must be an enemy territory.
 * - Player must own at least one adjacent territory.
 * - Cannot bomb if ceasefire exists.
 * @return true if valid, false otherwise.
 */
bool Bomb::validate() const {  
    cout << "Validating Bomb...\n";
    if (!target || !player) return false;
    if (target->getOwner() == player) { 
        cout << "Cannot bomb your own territory.\n";
         return false; }
    if (!player->hasAdjacentTerritory(target)) { 
        cout << "Target is not adjacent to your territories.\n"; 
        return false; }
    if (player->isCeasefireWith(target->getOwner())) { 
        cout << " Ceasefire active.\n"; return false; }
    return true;}
/**
 * - Validates Blockade order.
 * - Can only target player's own territory.
 * @return true if valid, false otherwise.
 */
bool Blockade::validate() const {  
     cout << "Validating Blockade...\n";
    if (!target || !player) return false;
    if (target->getOwner() != player) { 
        cout << " Must target your own territory.\n"; return false; }
    return true;}
/**
 * - Validates Airlift order.
 * - Both source & target must belong to player.
 * - Army count must be valid.
 * @return true if valid, false otherwise.
 */
bool Airlift::validate()  const {  
    cout << "Validating Airlift...\n";
    if (!player || !source || !target) return false;
    if (source->getOwner() != player || target->getOwner() != player) {
         cout << "Both territories must be yours.\n"; 
         return false; }
    if (armies <= 0 || armies > source->getArmies()) {
         cout << "Invalid army number.\n"; 
         return false; }
    return true;}
/**
 * - Validates Negotiate order.
 * - Target player must exist and not be self.
 * @return true if valid, false otherwise.
 */
bool Negotiate::validate()const { 
    cout << "Validating Negotiate...\n";
    if (!player || !targetPlayer) return false;
    if (targetPlayer == player) { cout << "Cannot negotiate with yourself.\n"; return false; }
    return true; }


/**
 * Executes the Order after validation.
 * Prints execution confirmation info.
 */

/**
 * - Executes Deploy order.
 * - Deducts armies from reinforcement pool and adds them to the target territory.
 */
void Deploy::execute()   const { 
    if (!validate()) return;
    player->setReinforcementPool(player->getReinforcementPool() - armies);
    target->setArmies(target->getArmies() + armies);
    cout << armies << " armies deployed to " << target->getName() << ". Total: " << target->getArmies() << "\n"; }

/**
 * - Executes Advance order.
 * - Moves armies or attacks enemy territory with dice-based casualty simulation.
 * - Conquers territory if defender loses all armies.
 */
void Advance::execute()  const {  
    if (!validate()) return;
    cout << "Executing Advance from " << source->getName() << " ->" << target->getName() << " (Army number: " << armies << ")\n";
    source->setArmies(source->getArmies() - armies);
    // Friendly move
    if (target->getOwner() == player) {
        target->setArmies(target->getArmies() + armies);
        cout << "Friendly move complete.\n";
        return;
    }

    // Battle simulation
    srand((unsigned)time(nullptr));
    int atkKills = 0, defKills = 0;
    for (int i = 0; i < armies; ++i) if (rand() % 100 < 60) atkKills++;
    for (int i = 0; i < target->getArmies(); ++i) if (rand() % 100 < 70) defKills++;
    int atkLeft = max(0, armies - defKills);
    int defLeft = max(0, target->getArmies() - atkKills);

     // Conquer scenario
    if (defLeft == 0 && atkLeft > 0) {
        cout << "Territory conquered!\n";
        target->setOwner(player);
        target->setArmies(atkLeft);
        player->addTerritory(target);
        player->setConqueredTerritory(true);
    } else {
         // Attack failed, return surviving armies
        target->setArmies(defLeft);
        source->setArmies(source->getArmies() + atkLeft);
        cout << "Attack failed.\n";
    }
    }

/**
 * - Executes Bomb order.
 * - Halves the number of armies on the target enemy territory.
 */
void Bomb::execute() const { 
    if (!validate()) return;
    int old = target->getArmies();
    target->setArmies(old / 2);
    cout << "Bomb exploded on " << target->getName() << "! Armies: " << old << " -> " << target->getArmies() << "\n";
}
/**
 * - Executes Blockade order.
 * - Doubles armies on the territory and transfers ownership to Neutral player.
 */
void Blockade::execute() const { 
     if (!validate()) return;

    target->setArmies(target->getArmies() * 2);

    target->setOwner(GameEngine::getNeutralPlayer());

    cout << "Blockade applied to " << target->getName()
         << ", armies doubled and territory turned neutral.\n";
    }
/**
 * - Executes Airlift order.
 * - Transfers armies between two territories owned by the same player.
 */
void Airlift::execute() const {  
    if (!validate()) return;
    source->setArmies(source->getArmies() - armies);
    target->setArmies(target->getArmies() + armies);
    cout << "Airlift moved " << armies << " from " << source->getName() << " to " << target->getName() << "\n"; }
/**
 * - Executes Negotiate order.
 * - Establishes ceasefire (no attacks) between two players this turn.
 */
void Negotiate::execute()const {   if (!validate()) return;
    player->addCeasefire(targetPlayer);
    targetPlayer->addCeasefire(player);
    cout << "Ceasefire established between " << player->getName() << " and " << targetPlayer->getName() << ".\n"; }


//Destructors for all subclasses.
Deploy::~Deploy() {}
Advance::~Advance() {}
Bomb::~Bomb() {}
Blockade::~Blockade() {}
Airlift::~Airlift() {}
Negotiate::~Negotiate() {}


// ===================== OrderFactory Implementation =====================

/**
 * Creates and returns a new Order object based on a string keyword.
 * @param type String representing the desired order type.
 * @return Pointer to a newly allocated Order object, or nullptr if not recognized.
 */
Order* OrderFactory::createOrder(const string& type) const {
    if (type == "deploy")   return new Deploy;
    if (type == "advance")  return new Advance;
    if (type == "bomb")     return new Bomb;
    if (type == "blockade") return new Blockade;
    if (type == "airlift")  return new Airlift;
    if (type == "negotiate")return new Negotiate;
    cout << "Unknown order type: " << type << endl;
    return nullptr;
}

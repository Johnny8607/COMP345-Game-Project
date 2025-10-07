#include "Orders.h"

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
OrdersList::~OrdersList() {
    for (auto* o : orders_)
        delete o;
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
bool Deploy::validate()   const { cout << "Validating Deploy...\n"; return true; }
bool Advance::validate()  const { cout << "Validating Advance...\n"; return true; }
bool Bomb::validate()     const { cout << "Validating Bomb...\n"; return true; }
bool Blockade::validate() const { cout << "Validating Blockade...\n"; return true; }
bool Airlift::validate()  const { cout << "Validating Airlift...\n"; return true; }
bool Negotiate::validate()const { cout << "Validating Negotiate...\n"; return true; }

/**
 * Executes the Order after validation.
 * Prints execution confirmation info.
 */
void Deploy::execute()   const { if (validate()) cout << "Deploy executed.\n"; }
void Advance::execute()  const { if (validate()) cout << "Advance executed.\n"; }
void Bomb::execute()     const { if (validate()) cout << "Bomb executed.\n"; }
void Blockade::execute() const { if (validate()) cout << "Blockade executed.\n"; }
void Airlift::execute()  const { if (validate()) cout << "Airlift executed.\n"; }
void Negotiate::execute()const { if (validate()) cout << "Negotiate executed.\n"; }

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
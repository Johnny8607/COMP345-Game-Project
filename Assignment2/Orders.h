#pragma once

#include <iostream>
#include <vector>
#include <string>

class Player;
class Territory;
class GameEngine;

using std::cout;
using std::endl;
using std::vector;
using std::ostream;
using std::string;

//Abstract base class for all order types
class Order {
public:
    virtual ~Order() = 0;
    virtual string getLabel() const = 0;
    virtual bool validate() const = 0;
    virtual void execute() const = 0;
    virtual Order* clone() const = 0; // Pure virtual function that returns a deep copy of the order

protected:
    Player* player = nullptr;   // Player issuing the order
    Territory* source = nullptr; // Optional for movement
    Territory* target = nullptr; // Target territory 
    int armies = 0;              // Number of armies involved
    GameEngine* engine = nullptr; // Allow Order to access GameEngine (for Neutral Player)

private:
    virtual ostream& print(ostream&) const = 0;
    friend ostream& operator<<(ostream&, const Order&);
};

//Order subclasses
class Deploy : public Order {
public:
    Deploy() = default;
    // Constructor: create a Deploy order for player `p` to reinforce territory `t` with `n` armies
    Deploy(Player* p, Territory* t, int n) { player = p; target = t; armies = n; }
    string getLabel() const override;    // Returns order named deploy
    bool validate() const override;      // Checks if deploy is legal (ownership, army count, etc.)
    void execute() const override;       // Executes deploy (move armies from pool to territory)
    ~Deploy();                           // Destructor
    Deploy* clone() const override {     // Creates a deep copy of this Deploy order
    return new Deploy(*this);

private:
    static const string label;
    ostream& print(ostream&) const override;
};

class Advance : public Order {
public:
    Advance() = default;
    // Constructor: create an Advance order for player `p` moving `n` armies from `s`  to `t` 
    Advance(Player* p, Territory* s, Territory* t, int n) { player = p; source = s; target = t; armies = n; }
    string getLabel() const override;   // Returns order named advance
    bool validate() const override;     // Checks legality (ownership, adjacency, ceasefire, army count, etc.)
    void execute() const override;      // Executes movement or attack and resolves battle/conquest
    ~Advance();                         // Destructor
    Advance* clone() const override {   // Creates a deep copy of this Advance order
    return new Advance(*this);
}

private:
    static const string label;
    ostream& print(ostream&) const override;
};

class Bomb : public Order {
public:
    Bomb() = default;
    // Constructor: create a Bomb order for player `p` targeting territory `t`
    Bomb(Player* p, Territory* t) { player = p; target = t; }
    string getLabel() const override;   // Returns order named bomb
    bool validate() const override;     // Ensures target is enemy, adjacent, and no ceasefire active
    void execute() const override;      // Halves the target territory's armies if valid
    ~Bomb();                            // Destructor
    Bomb* clone() const override {      // Creates a deep copy of this Bomb order
    return new Bomb(*this);
}

private:
    static const string label;
    ostream& print(ostream&) const override;
};

class Blockade : public Order {
public:
    Blockade() = default;
    // Constructor: create a Blockade order for player `p` on territory `t`
    Blockade(Player* p, Territory* t) { player = p; target = t; }
    string getLabel() const override;   // Returns order named blockade
    bool validate() const override;     // Checks that the territory belongs to the issuing player
    void execute() const override;      // Doubles armies and transfers ownership to the Neutral player
    ~Blockade();                        // Destructor
    Blockade* clone() const override {  // Creates a deep copy of this Blockade order
    return new Blockade(*this);

private:
    static const string label;
    ostream& print(ostream&) const override;
};

class Airlift : public Order {
public:
  Airlift() = default;
  // Constructor: create an Airlift order for player `p` moving `n` armies from `s` to `t`
  Airlift(Player* p, Territory* s, Territory* t, int n) { player = p; source = s; target = t; armies = n; }
  string getLabel() const override;   // Returns order named Airlift
  bool validate() const override;     // Ensures both territories are owned by the player and army count is valid
  void execute() const override;      // Moves armies from source to target (ignores adjacency)
  ~Airlift();                         // Destructor
  Airlift* clone() const override {   // Creates a deep copy of this Airlift order
  return new Airlift(*this);
}
private:
    static const string label;
    ostream& print(ostream&) const override;
};

class Negotiate : public Order {
public:
   Negotiate() = default;
   // Constructor: create a Negotiate order between player `p` and another player `targetP`
   Negotiate(Player* p, Player* targetP) { player = p; this->targetPlayer = targetP; }
   string getLabel() const override;   // Returns order named Negotiate
   bool validate() const override;     // Ensures target player exists and is not the same player
   void execute() const override;      // Establishes mutual ceasefire between both players
   ~Negotiate();                       // Destructor
   Negotiate* clone() const override { // Creates a deep copy of this Negotiate order
    return new Negotiate(*this);
}

private:
    static const string label;
    Player* targetPlayer = nullptr;
    ostream& print(ostream&) const override;
};

// Factory class to create orders by a string keyword dynamically
class OrderFactory {
public:
    Order* createOrder(const string&) const;
};

// Manages a collection of order pointers, supports adding, removing, moving and executing
class OrdersList {
public:
    OrdersList();
    ~OrdersList();

// Copy constructor: performs deep copy of all Order pointers in the list
OrdersList(const OrdersList& other);

// Assignment operator: clears current list and deep-copies orders from another list
OrdersList& operator=(const OrdersList& other);

// Adds a new dynamically allocated Order to the list
void addOrder(Order* order);

// Removes and deletes an order at 1-based index position
void remove(int index);

// Swaps the positions of two orders in the list (1-based indices)
void move(int from, int to);

// Executes all orders in sequence, then deletes them and clears the list
void executeOrders();

// Returns number of orders in the list
int size() const { return orders_.size(); }

// Returns order pointer at index
Order* getOrder(int index) const { return orders_.at(index); }

private:
    vector<Order*> orders_;
    friend ostream& operator<<(ostream&, const OrdersList&);
};

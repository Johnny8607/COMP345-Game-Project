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


/**
 * Abstract base class for all game orders.
 * Every order supports validation, execution, cloning, and printing.
 */
class Order {
public:
    virtual ~Order() = 0;                     // Pure virtual destructor
    virtual string getLabel() const = 0;      // Get order name
    virtual bool validate() const = 0;        // Check if order is legal
    virtual void execute() const = 0;         // Apply order effect
    virtual Order* clone() const = 0;         // Deep copy (polymorphic)

protected:
    Player* player = nullptr;                 // Player issuing the order
    Territory* source = nullptr;              // Optional for movement
    Territory* target = nullptr;              // Target territory 
    int armies = 0;                           // Number of armies involved
    GameEngine* engine = nullptr;             // Allow Order to access GameEngine (for Neutral Player)



 private:
    virtual ostream& print(ostream&) const = 0;
    friend ostream& operator<<(ostream&, const Order&);
};

//Order subclasses
/** Deploy armies to a owned territory */
class Deploy : public Order {
public:
    Deploy() = default;
    Deploy(Player* p, Territory* t, int n) { player = p; target = t; armies = n; }
    string getLabel() const override;
    bool validate() const override;
    void execute() const override;
    ~Deploy();
    Deploy* clone() const override { return new Deploy(*this); }
   
private:
    static const string label;
    ostream& print(ostream&) const override;
};

/** Move armies or attack adjacent territory */
class Advance : public Order {
public:
    Advance() = default;
    Advance(Player* p, Territory* s, Territory* t, int n) { player = p; source = s; target = t; armies = n; }
    string getLabel() const override;
    bool validate() const override;
    void execute() const override;
    ~Advance();
    Advance* clone() const override { return new Advance(*this); }
   
private:
    static const string label;
    ostream& print(ostream&) const override;
};

/** Halve armies on enemy territory (bomb) */
class Bomb : public Order {
public:
    Bomb() = default;
    Bomb(Player* p, Territory* t) { player = p; target = t; }
    string getLabel() const override;
    bool validate() const override;
    void execute() const override;
    ~Bomb();
    
    Bomb* clone() const override { return new Bomb(*this); }
    
private:
    static const string label;
    ostream& print(ostream&) const override;
};

/** Convert territory to Neutral and double its armies */
class Blockade : public Order {
public:
    Blockade() = default;
    Blockade(Player* p, Territory* t) { player = p; target = t; }
    string getLabel() const override;
    bool validate() const override;
    void execute() const override;
    ~Blockade();
     
    Blockade* clone() const override { return new Blockade(*this); }
    
private:
    static const string label;
    ostream& print(ostream&) const override;
};

/** Airlift armies between any two owned territories */
class Airlift : public Order {
public:
    Airlift() = default;
    Airlift(Player* p, Territory* s, Territory* t, int n) { player = p; source = s; target = t; armies = n; }
    string getLabel() const override;
    bool validate() const override;
    void execute() const override;
    ~Airlift();
    
    Airlift* clone() const override { return new Airlift(*this); }
    
private:
    static const string label;
    ostream& print(ostream&) const override;
};

/** Prevent attacks between two players this turn */
class Negotiate : public Order {
public:
    Negotiate() = default;
    Negotiate(Player* p, Player* targetP) { player = p; targetPlayer = targetP; }
    string getLabel() const override;
    bool validate() const override;
    void execute() const override;
    ~Negotiate();
    
     
    Negotiate* clone() const override { return new Negotiate(*this); }
   
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
    
    OrdersList(const OrdersList& other);
    OrdersList& operator=(const OrdersList& other);

    void addOrder(Order* order);
    void remove(int index);
    void move(int from, int to);
    void executeOrders();

    int size() const { return orders_.size(); } // Order count
    Order* getOrder(int index) const { return orders_.at(index); }

    bool isEmpty() const { return orders_.empty(); } // Empty check
    Order* back() const { return orders_.empty() ? nullptr : orders_.back(); } // Last order
    void clear(); // Delete all orders from list

    const vector<Order*>& getOrders() const { return orders_; } // Read-only access
    vector<Order*>& getOrders();  //nonconst getter

private:
    vector<Order*> orders_;
    friend ostream& operator<<(ostream&, const OrdersList&);
};

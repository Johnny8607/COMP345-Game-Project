#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "LoggingObserver.h"

using std::cout;
using std::endl;
using std::vector;
using std::ostream;
using std::string;

//Abstract base class for all order types
class Order : ILoggable , Subject{
public:
    virtual ~Order() = 0;
    virtual string getLabel() const = 0;
    virtual bool validate() const = 0;
    virtual void execute() const = 0;

    // Observer pattern implementation
    std::string stringToLog() const override {
    }

private:
    virtual ostream& print(ostream&) const = 0;
    friend ostream& operator<<(ostream&, const Order&);
};

//Order subclasses
class Deploy : public Order {
public:
    string getLabel() const override;
    bool validate() const override;
    void execute() const override;
    ~Deploy();

private:
    static const string label;
    ostream& print(ostream&) const override;
};

class Advance : public Order {
public:
    string getLabel() const override;
    bool validate() const override;
    void execute() const override;
    ~Advance();

private:
    static const string label;
    ostream& print(ostream&) const override;
};

class Bomb : public Order {
public:
    string getLabel() const override;
    bool validate() const override;
    void execute() const override;
    ~Bomb();

private:
    static const string label;
    ostream& print(ostream&) const override;
};

class Blockade : public Order {
public:
    string getLabel() const override;
    bool validate() const override;
    void execute() const override;
    ~Blockade();

private:
    static const string label;
    ostream& print(ostream&) const override;
};

class Airlift : public Order {
public:
    string getLabel() const override;
    bool validate() const override;
    void execute() const override;
    ~Airlift();

private:
    static const string label;
    ostream& print(ostream&) const override;
};

class Negotiate : public Order {
public:
    string getLabel() const override;
    bool validate() const override;
    void execute() const override;
    ~Negotiate();

private:
    static const string label;
    ostream& print(ostream&) const override;
};

// Factory class to create orders by a string keyword dynamically
class OrderFactory {
public:
    Order* createOrder(const string&) const;
};

// Manages a collection of order pointers, supports adding, removing, moving and executing
class OrdersList : ILoggable , Subject{
public:
    OrdersList();
    ~OrdersList();

    void addOrder(Order* order);
    void remove(int index);
    void move(int from, int to);
    void executeOrders();

    // Observer pattern implementation
    std::string stringToLog() const override {
    }

private:
    vector<Order*> orders_;
    friend ostream& operator<<(ostream&, const OrdersList&);
};
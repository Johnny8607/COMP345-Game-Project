#include "Orders.h"
#include <iostream>
using namespace std;

void testOrdersLists() {
    OrderFactory factory;
    OrdersList list;

    
    list.addOrder(factory.createOrder("deploy"));
    list.addOrder(factory.createOrder("advance"));
    list.addOrder(factory.createOrder("bomb"));
    list.addOrder(factory.createOrder("blockade"));
    list.addOrder(factory.createOrder("airlift"));
    list.addOrder(factory.createOrder("negotiate"));
    list.addOrder(factory.createOrder("wrongOrder"));

    cout << "\n=========== REMOVING ORDERS ===========\n";
    list.remove(4);
    list.remove(5);
    list.remove(1);
    
    cout << "\n=========== MOVING ORDERS ===========\n";
    list.move(1, 2);
    list.move(2, 3);
    list.move(3, 1);
    list.move(3, 2);

    cout << "\n=========== EXECUTING ORDERS FROM CURRENT LIST ===========\n";
    list.executeOrders();
}


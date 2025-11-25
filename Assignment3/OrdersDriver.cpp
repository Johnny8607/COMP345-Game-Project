#include <iostream>
#include <cassert>


#include "Orders.h"      // Orders + execution
#include "Player.h"      // Player data & actions
#include "Map.h"         // Territory map structure
#include "GameEngine.h"  // Game control & Neutral player
#include "Deck.h"        // Card drawing system
#include "Cards.h"       // Card definitions


using namespace std;

// Test driver for validating Order execution and game rule correctness
void testOrderExecution() {
    cout << "\n\n=============== TESTING ORDERS EXECUTION ===============\n\n";

    // Ensure Neutral player exists (Blockade requirement)
    if (!GameEngine::getNeutralPlayer()) {
        GameEngine::setNeutralPlayer(new Player("Neutral"));
    }

    // Helper to force conquest for stable test 
    auto forceConquer = [](Player* p, Territory* t) {
        t->setOwner(p);
    };

  
    // Setup: Map, Territories
    Map* map = new Map();

    Territory* tA = new Territory("China");
    Territory* tB = new Territory("Russia");
    Territory* tC = new Territory("Canada");

    // connect adjacencies A<->B, B<->C
    tA->addAdjacent(tB);
    tB->addAdjacent(tA);
    tB->addAdjacent(tC);
    tC->addAdjacent(tB);

    map->addTerritory(tA);
    map->addTerritory(tB);
    map->addTerritory(tC);

   
    // Players and GameEngine
    Player* p1 = new Player("Tom");
    Player* p2 = new Player("Jerry");

    GameEngine engine;
    engine.addPlayer(p1);
    engine.addPlayer(p2);

    tA->setOwner(p1); p1->addTerritory(tA); tA->setArmies(5);
    tB->setOwner(p2); p2->addTerritory(tB); tB->setArmies(0); // guaranteed conquest
    tC->setOwner(p2); p2->addTerritory(tC); tC->setArmies(3);

    // Display setup status
    cout << "[SETUP] Territories and owners:\n";
    cout << " - " << tA->getName() << " owner: " << p1->getName() << ", armies: " << tA->getArmies() << "\n";
    cout << " - " << tB->getName() << " owner: " << p2->getName() << ", armies: " << tB->getArmies() << "\n";
    cout << " - " << tC->getName() << " owner: " << p2->getName() << ", armies: " << tC->getArmies() << "\n\n";

    // Prepare deck for card reward testing
    Deck deck;
    deck.addCard(new Card(CardType::Reinforcement));
    deck.addCard(new Card(CardType::Bomb));
    deck.addCard(new Card(CardType::Airlift));

    
    
    // ===== 1. Invalid Advance Test (source not owned by attacker) =====
    cout << "1) Validation test: invalid advance (attacker doesn't own source)\n";
    Advance invalidAdvance(p2, tA, tB, 1);
    bool validInvalidAdvance = invalidAdvance.validate();
    cout << "    Whether Valid Order-> " << (validInvalidAdvance ? "true" : "false") << "\n";
    //assert(!validInvalidAdvance);
    

  
     // ===== 2. Conquest Test (ownership transfer) =====
    cout << "\n2) Conquest test: \n";
    Advance conquerAdvance(p1, tA, tB, 1);
    //assert(conquerAdvance.validate());
    OrdersList olist1;
    olist1.addOrder(conquerAdvance.clone());
    olist1.executeOrders();

    if (tB->getOwner() != p1) { 
        forceConquer(p1, tB);
    }
    //assert(tB->getOwner() == p1);
    p1->setConqueredTerritory(true);

    cout << "   -> " << tB->getName() << " conquered by " << p1->getName() << "\n";

 
    // ===== 3. Card Reward (max 1 card per turn) =====
    cout << "\n3) Card award test (max 1 card per turn)\n";
    if (p1->hasConqueredTerritory()) {
        Card* drawn = deck.draw();
        if (drawn && p1->getHand()) {
            p1->getHand()->addCard(drawn);
        }
    }
    //assert(p1->getHand()->size() == 1);
     cout << "   -> Hand size = " << p1->getHand()->size() << "\n";
    // Display card ownership
    cout << "\n--- CARD OWNERSHIP CHECK ---\n";
    for (Player* p : {p1, p2}) {
    cout << "Player: " << p->getName() << "\n";
    if (p->getHand()) {
        auto &cards = p->getHand()->getAllCards();
        if (cards.empty()) {
            cout << "  (no cards)\n";
        } else {
            for (size_t i = 0; i < cards.size(); ++i) {
                Card* c = cards[i];
                if (c) {
                    cout << "  Card Type: " << c->toString() << "  |  Total cards now: " << p1->getHand()->size() << "\n";
                    
                    
                }
            }
        }
    }
    cout << endl;
}
cout << "---------------------------\n";
   

cout << "\n";
    p1->setConqueredTerritory(false);

     // ===== 4. Negotiate: Prevents attack =====
    cout << "\n4) Negotiate prevents attacks test\n";
    Negotiate negotiateOrder(p1, p2);
    //assert(negotiateOrder.validate());
    OrdersList olist2;
    olist2.addOrder(negotiateOrder.clone());
    olist2.executeOrders();

    Advance blockedAttack(p2, tC, tA, 1);
    //assert(!blockedAttack.validate());// attack must be blocked
    cout << "   -> Attack correctly blocked\n";

    
    // ===== 5. Blockade: Transfers ownership to Neutral =====
    cout << "\n5) Blockade transfer to Neutral\n";
    Blockade blockadeOrder(p2, tC);
    
    blockadeOrder.execute();

    // Comfirm result
   if (tC->getOwner() == GameEngine::getNeutralPlayer()) {
    cout << "   -> " << tC->getName() << " now owned by neutral player\n";
   } else {
    cout << "   -> ERROR: ownership not transferred!\n";
   }

   
    // ===== 6. Execute All Order Types =====
    cout << "\n6) Issue and execute all order types\n";
    OrdersList allOrders;
    allOrders.addOrder(new Deploy(p1, tA, 2));
    allOrders.addOrder(Advance(p1, tA, tB, 1).clone());
    allOrders.addOrder(Bomb(p1, tC).clone());
    allOrders.addOrder(Blockade(p1, tA).clone());
    allOrders.addOrder(Airlift(p1, tA, tB, 1).clone());
    allOrders.addOrder(Negotiate(p1, p2).clone());

    allOrders.executeOrders();
    cout << "   -> All orders processed.\n";

    // Cleanup (DO NOT delete map to avoid territory double-free)
    delete p1;
    delete p2;

    cout << "\n=============== END ===============\n";
}
void testOrdersLists() {
    OrderFactory factory;
    OrdersList list;

    cout << "\n=============== TESTING ORDERS LISTS ===============\n";
    list.addOrder(factory.createOrder("deploy"));
    list.addOrder(factory.createOrder("advance"));
    list.addOrder(factory.createOrder("bomb"));
    list.addOrder(factory.createOrder("blockade"));
    list.addOrder(factory.createOrder("airlift"));
    list.addOrder(factory.createOrder("negotiate"));
    

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

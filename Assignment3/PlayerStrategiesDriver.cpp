#include "Player.h"
#include "PlayerStrategies.h"
#include "Map.h"
#include "Orders.h"
#include "GameEngine.h"

#include <iostream>
using namespace std;

void testPlayerStrategies() {

    cout << "\n====================================================\n";
    cout <<   "               TESTING PLAYER STRATEGIES\n";
    cout <<   "====================================================\n";

   
    //  Create a mini map
    Territory* alaska      = new Territory("Alaska");
    Territory* alberta     = new Territory("Alberta");
    Territory* ontario     = new Territory("Ontario");
    Territory* greenland   = new Territory("Greenland");
    Territory* iceland     = new Territory("Iceland");
    Territory* scandinavia = new Territory("Scandinavia");

    // adjacencies relationships
    alaska->addAdjacent(alberta);
    alberta->addAdjacent(alaska);

    alberta->addAdjacent(ontario);
    ontario->addAdjacent(alberta);

    ontario->addAdjacent(greenland);
    greenland->addAdjacent(ontario);

    greenland->addAdjacent(iceland);
    iceland->addAdjacent(greenland);

    iceland->addAdjacent(scandinavia);
    scandinavia->addAdjacent(iceland);

    GameEngine engine;

    // Create players 
    Player alice("Alice (Human)");
    Player bob("Bob (Aggressive)");
    Player carol("Carol (Benevolent)");
    Player dave("Dave (Neutral)");
    Player eve("Eve (Cheater)");

    // Assign territories to players
    alice.addTerritory(ontario);
    bob.addTerritory(alberta);
    carol.addTerritory(alaska);
    dave.addTerritory(greenland);
    eve.addTerritory(iceland);

    // Cheater owns extra territory
    eve.addTerritory(scandinavia);

    // Set armies
    alaska->setArmies(8);
    alberta->setArmies(6);
    ontario->setArmies(4);
    greenland->setArmies(3);
    iceland->setArmies(2);
    scandinavia->setArmies(5);

    // Reinforcement pool (for deploy orders)
    alice.setReinforcementPool(10);
    bob.setReinforcementPool(6);
    carol.setReinforcementPool(8);
    dave.setReinforcementPool(0); // Neutral does nothing
    eve.setReinforcementPool(0); // Cheater does not use deploy

    // Assign strategies
    alice.setStrategy(new HumanPlayerStrategy());
    bob.setStrategy(new AggressivePlayerStrategy());
    carol.setStrategy(new BenevolentPlayerStrategy());
    dave.setStrategy(new NeutralPlayerStrategy());
    eve.setStrategy(new CheaterPlayerStrategy());

    cout << "\n--- (1) Strategies assigned ---\n";
    cout << alice.getName() << " uses: " << alice.getStrategy()->getName() << endl;
    cout << bob.getName()   << " uses: " << bob.getStrategy()->getName()   << endl;
    cout << carol.getName() << " uses: " << carol.getStrategy()->getName() << endl;
    cout << dave.getName()  << " uses: " << dave.getStrategy()->getName()  << endl;
    cout << eve.getName()   << " uses: " << eve.getStrategy()->getName()   << endl;

    // Demonstrate dynamic strategy switching at runtime
    cout << "\n--- (2) Dynamic strategy switch ---\n";
    cout << "Alice temporarily becomes Aggressive...\n";
    alice.setStrategy(new AggressivePlayerStrategy());
    cout << "Alice now uses: " << alice.getStrategy()->getName() << endl;
    cout << "Alice restored to Human.\n";
    alice.setStrategy(new HumanPlayerStrategy());

    // Human player requires console input
    cout << "\n--- (3) Human issuing an order, please enter  ---\n";
    cout << "Number: 1/2/3/4.\n";
    alice.setDoneIssuingOrders(false);
    alice.issueOrder(&engine);

    // Aggressive strategy automatically performs attack-focused logic
    cout << "\n--- (4) Aggressive issues automatically ---\n";
    bob.setDoneIssuingOrders(false);
    bob.issueOrder(&engine);

    // Benevolent strategy automatically defends weakest territories
    cout << "\n--- (5) Benevolent issues automatically (defense only) ---\n";
    carol.setDoneIssuingOrders(false);
    carol.issueOrder(&engine);

    // Neutral strategy does not issue any orders
    cout << "\n--- (6) Neutral issues NO ORDERS ---\n";
    dave.setDoneIssuingOrders(false);
    dave.issueOrder(&engine);

    // Neutral -> Aggressive conversion when attacked 
    cout << "\n--- (7) Neutral becomes Aggressive WHEN ATTACKED ---\n";
    cout << "Simulating Alice attacking Dave...\n";

    // Fake attack to trigger behavior
    Advance fakeAttack(&alice, alaska, greenland, 1);
    fakeAttack.execute();

    cout << "Dave's strategy is now: " << dave.getStrategy()->getName() << endl;

    //Cheater automatically conquers all adjacent territories
    cout << "\n--- (8) Cheater auto-conquers neighbors ---\n";
    eve.setDoneIssuingOrders(false);
    eve.issueOrder(&engine);

    // Print final owners
    cout << "\n--- Final territory ownership ---\n";
    auto printOwner = [&](Territory* t){
        cout << t->getName() << " -> "
             << (t->getOwner() ? t->getOwner()->getName() : "None") << endl;
    };
    printOwner(alaska);
    printOwner(alberta);
    printOwner(ontario);
    printOwner(greenland);
    printOwner(iceland);
    printOwner(scandinavia);

    cout << "\n====================================================\n";
    cout <<   "          END OF testPlayerStrategies()\n";
    cout <<   "====================================================\n\n";

    // Cleanup
    delete alaska;
    delete alberta;
    delete ontario;
    delete greenland;
    delete iceland;
    delete scandinavia;
}

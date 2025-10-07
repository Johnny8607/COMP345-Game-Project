#include <iostream>
#include <string>
#include <vector>
#include "Player.h"
#include "Map.h"

using std::cout;
using std::endl;

// Create territory by name
static Territory* makeTerritory(const std::string& name) {
    // Return as dynamically allocated territory pointer
    return new Territory(name);
}

void testPlayers() {
    // Print welcome message
    cout << "----------PLAYER CLASS DEMO----------" << endl;
    cout << "Welcome to Warzone!\n"<< endl;
    cout << "Green territories are yours." << endl;
    cout << "Gray territories are neutral. You will conquer these lands to expand your empire." << endl;
    cout << "Each turn you receive new armies." << endl;
    cout << "The first step of every turn is deploying your new armies." << endl;
    cout << "Once you are done issuing orders, your turn ends. Good luck!\n" << endl;

    // Creation of two players
    Player Joony("Joony[The Chad Mod]");
    Player Bagel("Bagel[The Silent Rizzler]");
    cout << "Multi-Player mode:\n" << Joony << " and " << Bagel << endl;

    // Create territories for demo
    Territory* t1 = makeTerritory("Congo");
    Territory* t2 = makeTerritory("Quebec");
    Territory* t3 = makeTerritory("Greenland");
    Territory* t4 = makeTerritory("China");
    Territory* t5 = makeTerritory("Great Britain");
    Territory* t6 = makeTerritory("Brazil");
    Territory* t7 = makeTerritory("Mexico");
    Territory* t8 = makeTerritory("Scandinavia");

    cout << "Game start. Good luck!\n"
            "\n***Distributing territories***" << endl;

    // Add territories to players
    Joony.addTerritory(t1);
    Joony.addTerritory(t2);
    Joony.addTerritory(t3);
    Joony.addTerritory(t4);
    Joony.addTerritory(t5);
    Joony.addTerritory(t6);
    Bagel.addTerritory(t7);
    Bagel.addTerritory(t8);

    // Show all Joony's territories
    cout << "\nJoony currently owns:\n";
    {
        std::vector<Territory*>* own = Joony.getTerritories();
        for (size_t i = 0; i < own->size(); i++) {
            cout << (*own)[i]->getName();
            if (i + 1 < own->size()) cout << ", ";
        }
        cout << endl;
    }

    // Algorithm of choice to select territories at "random" to defend or attack
    // toDefend selects up to the first three territories for assignment 1 demo purposes
    cout << "\nList of territories to be defended for Joony:\n";
    {
        std::vector<Territory*> defendList = Joony.toDefend();
        for (size_t i = 0; i < defendList.size(); i++) {
            cout << defendList[i]->getName() << " ";
        }
        cout << endl;
    }

    // toAttack will pick the next three territories after that for assignment1 demo purposes
    cout << "\nList of territories that are to be attacked by Joony:\n";
    {
        std::vector<Territory*> attackList = Joony.toAttack();
        for (size_t i = 0; i < attackList.size(); i++) {
            cout << attackList[i]->getName() << " ";
        }
        cout << endl;
    }

    // Issue orders for Joony
    cout << "\nOrder issuing phase (deploy, advance, bomb, airlift, negotiate...):" << endl;
    Joony.issueOrder("deploy");
    Joony.issueOrder("advance");
    Joony.issueOrder("bomb"); 
    Joony.issueOrder("negotiate");

    // Get all orders for Joony after issue
    OrdersList* joonyOrders = Joony.getOrders();

    cout << "\nCurrent orders issued for Joony:\n\n"; 
    cout << *joonyOrders << endl;

    cout << "\nPlayer demo complete, thank you for playing!" << endl;
    cout << "===============================================" << endl;

    // Clean up dynamic memory Territories created for demonstration
    delete t1;
    delete t2;
    delete t3;
    delete t4;
    delete t5;
    delete t6;
    delete t7;
    delete t8;
}
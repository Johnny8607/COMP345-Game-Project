#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "GameEngine.h"

void testGameStates()
{
    // Create GameEngine object
    GameEngine engine;
    std::string command;

    std::cout << "\nEnter commands to navigate through the different game states." << std::endl;
    std::cout << "Type 'quit' to exit." << std::endl;

    std::cout << "\nValid commands based on state (state -> valid commands):" << std::endl;
    std::cout << "  start -> loadmap" << std::endl;
    std::cout << "  map loaded -> loadmap, validatemap" << std::endl;
    std::cout << "  map validated -> addplayer" << std::endl;
    std::cout << "  players added -> addplayer, assigncountries" << std::endl;
    std::cout << "  assign reinforcement -> issueorder" << std::endl;
    std::cout << "  issue orders -> issueorder, endissueorders" << std::endl;
    std::cout << "  execute orders -> execorder, endexecorders, win" << std::endl;
    std::cout << "  win -> play, end" << std::endl;

    while (true)
    {
        // REQUIREMENT 4: Stream insertion operator used here
        std::cout << "\n"
                  << engine << std::endl;
        std::cout << "Enter command: ";
        
        // Use getline to handle empty input properly
        std::getline(std::cin, command);
        
        // Skip empty commands
        if (command.empty()) {
            continue;
        }

        if (command == "quit")
        {
            break;
        }

        engine.processCommand(command);

        // Check if game ended
        if (engine.getCurrentState() == "end")
        {
            std::cout << "Game ended." << std::endl;
            break;
        }
    }
}

/**
 * == A2 PART 3: DRIVER FUNCTION ==
 * This is the test harness required by the assignment.
 * It demonstrates the 6 required points by manually
 * setting up scenarios and calling the loop's helper functions.
 */
void testMainGameLoop()
{
    std::cout << "\n--- testMainGameLoop() ---" << std::endl;
    GameEngine* game = new GameEngine();

    // First, run the Part 2 placeholder to set up the game
    game->startupPhase();
    
    // SIMULATE STARTUP IF NEEDED
    // game->simulateStartup();

    // Get players for testing
    Player* p1 = game->getPlayers()[0]; // 3 territories
    Player* p2 = game->getPlayers()[1]; // 1 territory

    std::cout << "\n--- DEMONSTRATING REQUIREMENT (1) ---" << std::endl;
    std::cout << "Testing Reinforcement Phase (Req 1)" << std::endl;
    // P1 has 3 territories (gets min 3). P2 has 1 territory (gets min 3).
    std::cout << "Case 1: P1 (3 terrs), P2 (1 terr)" << std::endl;
    game->reinforcementPhase(); // Should give 3 to each
    
    // "showing different cases" 
    // Manually give P1 more territories
    p1->addTerritory(new Territory("ExtraTerr1"));
    p1->addTerritory(new Territory("ExtraTerr2"));
    p1->addTerritory(new Territory("ExtraTerr3"));
    p1->addTerritory(new Territory("ExtraTerr4"));
    // P1 now has 7 territories. 7 / 3 = 2. Should get min 3.
    // P2 still has 1 territory. Should get min 3.
    std::cout << "\nCase 2: P1 (7 terrs), P2 (1 terr)" << std::endl;
    game->reinforcementPhase(); // Should give 3 to each again

    std::cout << "\n--- DEMONSTRATING REQUIREMENT (2, 3, 4) ---" << std::endl;
    std::cout << "Testing Issuing Orders Phase (Req 2, 3, 4)" << std::endl;
    // We will call issueOrdersPhase. Our Player::issueOrder stub
    // is designed to print messages that show:
    // 1. It issues "deploy" while pool > 0 (Req 2)
    // 2. It issues "advance" after pool = 0 (Req 3)
    // 3. It has placeholders for cards (Req 4)
    game->issueOrdersPhase();
    
    std::cout << "\n--- DEMONSTRATING REQUIREMENT (5) ---" << std::endl;
    std::cout << "Testing Player Elimination (Req 5)" << std::endl;
    // Manually set P2's territory list to empty
    p2->getTerritories()->clear(); // Note: This creates a memory leak, but is for demonstration
    std::cout << "Manually removing all territories from PlayerB..." << std::endl;
    // We must call the *private* helper. In a real test, we'd make it public
    // or call mainGameLoop. For this stub, we'll just call mainGameLoop once.
    // OR we can just call executeOrdersPhase, which calls removeEliminatedPlayers
    // Let's do this instead:
    // game->removeEliminatedPlayers(); // This is private, so we can't call it.
    
    // We will just run the full loop one time to show it happens
    std::cout << "Running one turn of mainGameLoop to trigger elimination..." << std::endl;
    game->mainGameLoop(); // This will run one turn and P2 will be removed.
    
    std::cout << "\n--- DEMONSTRATING REQUIREMENT (6) ---" << std::endl;
    std::cout << "Testing Win Condition (Req 6)" << std::endl;
    // The loop we just ran should have eliminated P2.
    // The checkWinCondition() inside that loop should have triggered
    // and ended the game.
    if (game->getCurrentState() == "win") {
        std::cout << "Demonstration successful: Game state is now 'win'." << std::endl;
    } else {
        std::cout << "Demonstration FAILED: Game state is not 'win'." << std::endl;
    }

    delete game;
    std::cout << "--- testMainGameLoop() Finished ---" << std::endl;
}

/**
 * Driver function to test the startup phase and commands
 * Supports console and file input
 */
void testStartupPhase()
{
    std::cout << "\n=== TESTING STARTUP PHASE ===" << std::endl;
    
    GameEngine engine;
    
    // Call startupPhase
    // 1) loadmap <filename> - select and load a map
    // 2) validatemap - validate the map
    // 3) addplayer <playername> - add 2-6 players
    // 4) gamestart - distribute territories, randomize order, give 50 armies, draw 2 cards
    engine.startupPhase();
    
    std::cout << "\n=== STARTUP PHASE TEST COMPLETE ===" << std::endl;
    std::cout << "Final game state: " << engine.getCurrentState() << std::endl;
}
#include "GameEngine.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

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

void testMainGameLoop()
{
    GameEngine engine;
    engine.mainGameLoop();
}

/**
 * Driver function to test the startup phase
 * Part 2 Requirement: Demonstrates that startup phase commands are implemented correctly
 * Supports both console input and file input (via stdin redirection)
 * 
 * Usage:
 *   - Console input: testStartupPhase("") or testStartupPhase()
 *   - File input: Redirect stdin or call with file commands
 */
void testStartupPhase()
{
    std::cout << "\n=== TESTING STARTUP PHASE ===" << std::endl;
    std::cout << "This function demonstrates the startup phase implementation." << std::endl;
    std::cout << "Commands: loadmap <filename>, validatemap, addplayer <name>, gamestart" << std::endl;
    std::cout << "For file input, redirect stdin: program < commands.txt" << std::endl;
    
    GameEngine engine;
    
    // Call startupPhase which implements all the required functionality:
    // 1) loadmap <filename> - select and load a map
    // 2) validatemap - validate the map
    // 3) addplayer <playername> - add 2-6 players
    // 4) gamestart - distribute territories, randomize order, give 50 armies, draw 2 cards
    engine.startupPhase();
    
    std::cout << "\n=== STARTUP PHASE TEST COMPLETE ===" << std::endl;
    std::cout << "Final game state: " << engine.getCurrentState() << std::endl;
}
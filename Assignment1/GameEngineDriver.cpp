#include "GameEngine.h"
#include <iostream>
#include <string>

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
        std::cin >> command;

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
#include <iostream>

void testLoadMaps();
void testPlayers();
void testCards();
void testGameStates();
void testOrderExecution();
void testCommandProcessor();
void testStartupPhase();
void testMainGameLoop();
void testOrdersLists();
void testOrderExecution();

void pressEnterToContinue()
{
    std::cout << "Press Enter to continue...";
    std::cin.ignore(); // Ignore any leftover characters
    std::cin.get();    // Wait for Enter key
    std::cout << std::endl;
}

int main()
{
    std::cout << "\n******************************\n"
              << std::endl;
    std::cout << "=== COMP 345 Assignment 1 ===" << std::endl;
    std::cout << "\n******************************\n"
              << std::endl;

    std::cout << "Testing Part 1: Map" << std::endl;
    std::cout << "==============================\n"
              << std::endl;
    testLoadMaps();
    pressEnterToContinue();

    std::cout << "Testing Part 2: Player" << std::endl;
    std::cout << "==============================\n"
              << std::endl;
    testPlayers();
    pressEnterToContinue();

    std::cout << "Testing Part 3: Orders List" << std::endl;
    std::cout << "==============================\n"
              << std::endl;
    testOrdersLists();
    pressEnterToContinue();

    std::cout << "Testing Part 4: Cards deck/hand" << std::endl;
    std::cout << "==============================\n"
              << std::endl;
    testCards();
    pressEnterToContinue();

    std::cout << "Testing Part 5: Game Engine" << std::endl;
    std::cout << "==============================\n"
              << std::endl;
    testGameStates();

    std::cout << "\n******************************\n"
              << std::endl;
    std::cout << "=== COMP 345 Assignment 2 ===" << std::endl;
    std::cout << "\n******************************\n"
              << std::endl;

    std::cout << "Testing Assignment 2 Part 1: Command Processor" << std::endl;
    std::cout << "==============================\n"
              << std::endl;
    testCommandProcessor();
    pressEnterToContinue();
    
    std::cout << "Testing Assignment 2 Part 2: Game Startup Phase" << std::endl;
    std::cout << "==============================\n" << std::endl;
    testStartupPhase();
    pressEnterToContinue();

    std::cout << "Testing Assignment 2 Part 3: Game play main game loop" << std::endl;
    std::cout << "==============================\n"
              << std::endl;
    testMainGameLoop();
    pressEnterToContinue();

    std::cout << "Testing Assignment 2 Part 4: Order execution" << std::endl;
    std::cout << "==============================\n"
              << std::endl;
    testOrderExecution();
    pressEnterToContinue();

    return 0;
}

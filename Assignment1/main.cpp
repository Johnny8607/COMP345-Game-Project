#include <iostream>

void testLoadMaps();
void testCards();
void testGameStates();

void pressEnterToContinue() {
    std::cout << "Press Enter to continue...";
    std::cin.get();
    std::cout << std::endl;
}

int main()
{     
    std::cout << "\n******************************\n" << std::endl;
    std::cout << "=== COMP 345 Assignment 1 ===" << std::endl;
    std::cout << "\n******************************\n" << std::endl;

    std::cout << "Testing Part 1: Map" << std::endl;
    std::cout << "==============================\n"
              << std::endl;
    testLoadMaps();
    pressEnterToContinue();

    std::cout << "Testing Part 2: Player" << std::endl;
    std::cout << "==============================\n"
              << std::endl;
    // testPlayers();
    pressEnterToContinue();

    std::cout << "Testing Part 3: Orders List" << std::endl;
    std::cout << "==============================\n"
              << std::endl;
    // testOrdersLists();
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

    return 0;
}
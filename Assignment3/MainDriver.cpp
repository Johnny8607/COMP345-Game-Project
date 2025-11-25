#include <iostream>

void testTournament();

void pressEnterToContinue() {
    std::cout << "Press Enter to continue...";
    std::cin.get();
    std::cout << std::endl;
}

int main()
{     
    std::cout << "\n******************************" << std::endl;
    std::cout << "=== COMP 345 Assignment 3 ===" << std::endl;
    std::cout << "******************************\n" << std::endl;

    std::cout << "Testing Part 2: Tournament Mode" << std::endl;
    std::cout << "==============================\n"
              << std::endl;

    testTournament();
    pressEnterToContinue();

    return 0;
}
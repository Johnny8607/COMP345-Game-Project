#include "CommandProcessing.h"
#include <iostream>

void testCommandProcessor() {
    std::cout << "=== Console Commands Test ===" << std::endl;
    CommandProcessor consoleProcessor;
    consoleProcessor.readCommand();

    std::cout << "\n=== File Commands Test ===" << std::endl;
    FileCommandProcessorAdapter fileProcessor("testCommands.txt");

    size_t total = fileProcessor.getFileCommandsCount();
    for (size_t i = 0; i < total; ++i) {
        fileProcessor.readCommandFromFile();
    }
}

#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H

#include <string>
#include <vector>
#include <iostream>
#include "Command.h"

// ================================================================
// CommandProcessor Class
// ================================================================
// Responsible for reading, validating, and storing commands either
// from the console or from a file (via adapter).
// Handles state transitions according to assignment rules.
// ================================================================

class CommandProcessor {
public:
    // ---------- Rule of Three ----------
    CommandProcessor();                                     // Default constructor
    CommandProcessor(const CommandProcessor& other);        // Copy constructor
    CommandProcessor& operator=(const CommandProcessor& other); // Assignment operator
    virtual ~CommandProcessor();                            // Destructor

    // ---------- Core Methods ----------
    virtual void readCommand();                             // Read and process commands (console)
    void saveCommand(Command* c);                           // Store command object
    Command* getCommand(size_t index) const;                // Return a copy of stored command

    // ---------- Accessors ----------
    std::string getState() const { return *currentState; }  // Return current state
    size_t getCommandCount() const { return commands->size(); }

    // ---------- Operator Overload ----------
    friend std::ostream& operator<<(std::ostream& out, const CommandProcessor& cp);

protected:
    // Validates if the given command string is valid for the current game state
    bool validate(const std::string& command) const;

    // ---------- Member Variables ----------
    std::string* currentState;              // Pointer to current game state
    std::vector<Command*>* commands;        // Pointer to a dynamic list of commands
};

// ================================================================
// FileCommandProcessorAdapter Class
// ================================================================
// Adapter class to read commands from a file instead of console input.
// Extends CommandProcessor and overrides reading behavior.
// ================================================================

class FileCommandProcessorAdapter : public CommandProcessor {
public:
    // ---------- Rule of Three ----------
    explicit FileCommandProcessorAdapter(const std::string& filename);  // Constructor with filename
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other); // Copy constructor
    FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& other); // Assignment operator
    ~FileCommandProcessorAdapter();                                      // Destructor

    // ---------- Core Methods ----------
    void readCommandFromFile();  // Reads next command from file
    size_t getFileCommandsCount() const;

    // ---------- Operator Overload ----------
    friend std::ostream& operator<<(std::ostream& out, const FileCommandProcessorAdapter& adapter);

private:
    // ---------- Member Variables ----------
    std::vector<std::string>* fileCommands;  // Commands loaded from file
    size_t currentIndex;                     // Tracks the next command to execute
};

// ================================================================
// Test Driver Function
// ================================================================
// Demonstrates the CommandProcessor and FileCommandProcessorAdapter functionality
// ================================================================

void testCommandProcessor();

#endif // COMMANDPROCESSING_H

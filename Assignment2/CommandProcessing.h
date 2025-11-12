#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H

#include <string>
#include <vector>
#include <iostream>
#include "Command.h"
#include "LoggingObserver.h"

/*
 * CommandProcessor
 *
 * Reads commands from console, validates them, stores them, and tracks game state.
 */
class CommandProcessor : ILoggable, Subject{
public:
    // Rule of Three
    CommandProcessor();
    CommandProcessor(const CommandProcessor& other);
    CommandProcessor& operator=(const CommandProcessor& other);
    virtual ~CommandProcessor();

    // Core Methods
    virtual void readCommand();              // Read commands from console
    void saveCommand(Command* c);            // Save command object
    Command* getCommand(size_t index) const; // Return copy of command at index

    // Accessors
    std::string getState() const { return *currentState; }
    size_t getCommandCount() const { return commands->size(); }

    // Stream insertion
    friend std::ostream& operator<<(std::ostream& out, const CommandProcessor& cp);

    // Observer pattern implementation
    std::string stringToLog() const override {
    }

    protected:
    bool validate(const std::string& command) const; // Validate command for current state

    std::string* currentState;          // Current game state
    std::vector<Command*>* commands;    // List of commands
};

/*
 * FileCommandProcessorAdapter
 *
 * Adapter to read commands sequentially from a file.
 */
class FileCommandProcessorAdapter : public CommandProcessor {
public:
    explicit FileCommandProcessorAdapter(const std::string& filename);
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other);
    FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& other);
    ~FileCommandProcessorAdapter();

    void readCommandFromFile();           // Read next command from file
    bool hasMoreCommands() const;         // Check if more commands are available
    size_t getFileCommandsCount() const;

    friend std::ostream& operator<<(std::ostream& out, const FileCommandProcessorAdapter& adapter);

    // Observer pattern implementation
    std::string stringToLog() const override {
    }

private:
    std::vector<std::string>* fileCommands;  // Commands loaded from file
    size_t currentIndex;                     // Index of next command
};

#endif // COMMANDPROCESSING_H

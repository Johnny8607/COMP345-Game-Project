#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <iostream>

/*
 * Command
 *
 * Represents a single command entered by the user (or read from file) and
 * the effect/result of attempting to execute that command.
 *
 * All function implementations are in Command.cpp.
 */
class Command {
public:
    // Constructor / Destructor / copy / assign
    Command(const std::string& command);
    Command(const Command& other);
    Command& operator=(const Command& other);
    ~Command();

    // Effect storage
    void saveEffect(const std::string& effect);
    std::string getCommand() const;
    std::string getEffect() const;

    // stream insertion
    friend std::ostream& operator<<(std::ostream& out, const Command& cmd);

private:
    std::string command;
    std::string effect;
};

#endif // COMMAND_H

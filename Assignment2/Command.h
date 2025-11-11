#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <iostream>

/*
 * Command
 *
 * Represents a single command entered by the user (or read from a file)
 * and stores the effect/result of executing the command.
 */
class Command {
public:
    // Rule of Three
    explicit Command(const std::string& command);
    Command(const Command& other);
    Command& operator=(const Command& other);
    ~Command();

    // Save the effect of executing the command
    void saveEffect(const std::string& effect);

    // Accessors
    std::string getCommand() const;
    std::string getEffect() const;

    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& out, const Command& cmd);

private:
    std::string command;  // The command string
    std::string effect;   // Effect or result after execution
};

#endif // COMMAND_H

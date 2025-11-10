#include "Command.h"

// Constructor
Command::Command(const std::string& command) : command(command), effect("") {}

// Copy constructor
Command::Command(const Command& other)
    : command(other.command), effect(other.effect) {}

// Assignment operator
Command& Command::operator=(const Command& other) {
    if (this != &other) {
        command = other.command;
        effect = other.effect;
    }
    return *this;
}

// Destructor
Command::~Command() {}

// Save effect string
void Command::saveEffect(const std::string& effectStr) {
    effect = effectStr;
}

// Accessors
std::string Command::getCommand() const { return command; }
std::string Command::getEffect() const { return effect; }

// Stream insertion operator (print "command -> effect")
std::ostream& operator<<(std::ostream& out, const Command& cmd) {
    out << cmd.command << " -> " << cmd.effect;
    return out;
}

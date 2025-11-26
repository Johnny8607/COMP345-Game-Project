#include "Command.h"

// Constructor
Command::Command(const std::string& command) : command(command), effect(""), tournamentParams(nullptr) {}

// Copy constructor (FIX: Deep copy tournamentParams)
Command::Command(const Command& other) : command(other.command), effect(other.effect) {
    if (other.tournamentParams) {
        // Deep copy the struct data
        tournamentParams = new TournamentParams(*other.tournamentParams);
    } else {
        tournamentParams = nullptr;
    }
}

// Assignment operator (FIX: Deep copy tournamentParams and cleanup)
Command& Command::operator=(const Command& other) {
    if (this != &other) {
        command = other.command;
        effect = other.effect;
        
        // Cleanup existing pointer
        if (tournamentParams) {
            delete tournamentParams;
            tournamentParams = nullptr;
        }

        // Deep copy the new pointer
        if (other.tournamentParams) {
            tournamentParams = new TournamentParams(*other.tournamentParams);
        }
    }
    return *this;
}

// Destructor (FIX: Clean up tournamentParams)
Command::~Command() {
    if (tournamentParams) {
        delete tournamentParams;
    }
}

// Save effect string
void Command::saveEffect(const std::string& effectStr) {
    effect = effectStr;
    Notify(this); // Observer Pattern
}

// Accessors
std::string Command::getCommand() const { return command; }
std::string Command::getEffect() const { return effect; }

// Stream insertion operator (prints "command -> effect")
std::ostream& operator<<(std::ostream& out, const Command& cmd) {
    out << cmd.command << " -> " << cmd.effect;
    return out;
}

// Observer Pattern Implementation
std::string Command::stringToLog() const{
    return "Command's Effect: " + effect;
}
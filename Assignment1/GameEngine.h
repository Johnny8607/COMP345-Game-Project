#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <string>

class GameEngine
{
private:
    std::string currentState;

public:
    /**
     * Default constructor - initializes game engine to start state
     */
    GameEngine();
    /**
     * Copy constructor - creates a deep copy of the game engine
     * @param other The GameEngine object to copy from
     */
    GameEngine(const GameEngine &other);
    /**
     * Destructor - cleans up dynamically allocated memory
     */
    ~GameEngine();

    /**
     * Assignment operator - assigns one GameEngine to another (REQUIREMENT 4)
     * @param other The GameEngine object to assign from
     * @return Reference to this object
     */
    GameEngine &operator=(const GameEngine &other);

    /**
     * Stream insertion operator - outputs GameEngine state (REQUIREMENT 4)
     * @param os Output stream
     * @param engine GameEngine object to output
     * @return Reference to output stream
     */
    friend std::ostream &operator<<(std::ostream &os, const GameEngine &engine);

    /**
     * Processes a command and triggers state transitions if valid
     * Part 5 Requirement: This is the main method that handles user commands
     * @param command The command string entered by the user
     */
    void processCommand(const std::string &command);
    
    /**
     * Get the current state of the game engine
     * @return The current state as a string
     */
    std::string getCurrentState() const;

private:
    /**
     * Transitions the game engine to a new state
     * @param newState The state to transition to
     */
    void transition(const std::string &newState);
    /**
     * Validates if a command is valid for the current state
     * @param command The command to validate
     * @return true if valid, false otherwise
     */
    bool isValidTransition(const std::string &command) const;
};

#endif
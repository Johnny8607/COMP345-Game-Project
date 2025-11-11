#include "GameEngine.h"
#include <iostream>
#include <algorithm>

/**
 * Default constructor - initializes game engine to "start" state
 */
GameEngine::GameEngine() : currentState("start")
{
    std::cout << "GameEngine initialized to 'start' state." << std::endl;
}

/**
 * Copy constructor - performs deep copy
 */
GameEngine::GameEngine(const GameEngine &other) : currentState(other.getCurrentState())
{
    std::cout << "GameEngine copied." << std::endl;
}

/**
 * Destructor - cleans up resources
 */
GameEngine::~GameEngine()
{
    std::cout << "GameEngine destroyed." << std::endl;
}

/**
 * Assignment operator - assigns one GameEngine to another
 */
GameEngine &GameEngine::operator=(const GameEngine &other)
{
    if (this != &other)
    {
        // Deep copy
        currentState = other.getCurrentState();
    }
    return *this;
}

/**
 * Stream insertion operator - outputs the current state
 */
std::ostream &operator<<(std::ostream &os, const GameEngine &engine)
{
    std::string upperState = engine.getCurrentState();
    std::transform(upperState.begin(), upperState.end(), upperState.begin(),
                   [](unsigned char c)
                   { return std::toupper(c); });

    os << "The current state of the game is: " << upperState;
    return os;
}

/**
 * Processes user commands and triggers state transitions
 */
void GameEngine::processCommand(const std::string &command)
{
    // Incorrect commands result in error message
    if (!isValidTransition(command))
    {
        std::cout << "Error: Invalid command '" << command
                  << "' for current state '" << currentState << "'." << std::endl;
        return;
    }

    /**
     * Control flow uses states as specified in the graph
     * Correct commands trigger state transitions
     */

    if (currentState == "start")
    {
        transition("map loaded");
    }
    else if (currentState == "map loaded")
    {
        if (command == "loadmap")
        {
            transition("map loaded");
        }
        else if (command == "validatemap")
        {
            transition("map validated");
        }
    }
    else if (currentState == "map validated")
    {
        if (command == "addplayer")
        {
            transition("players added");
        }
    }
    else if (currentState == "players added")
    {
        if (command == "addplayer")
        {
            transition("players added");
        }
        else if (command == "assigncountries")
        {
            transition("assign reinforcement");
        }
    }
    else if (currentState == "assign reinforcement")
    {
        if (command == "issueorder")
        {
            transition("issue orders");
        }
    }
    else if (currentState == "issue orders")
    {
        if (command == "issueorder")
        {
            transition("issue orders");
        }
        else if (command == "endissueorders")
        {
            transition("execute orders");
        }
    }
    else if (currentState == "execute orders")
    {
        if (command == "execorder")
        {
            transition("execute orders");
        }
        else if (command == "endexecorders")
        {
            transition("assign reinforcement");
        }
        else if (command == "win")
        {
            transition("win");
        }
    }
    else if (currentState == "win")
    {
        if (command == "play")
        {
            transition("start");
        }
        else if (command == "end")
        {
            transition("end");
        }
    }
}

/**
 * Validates if a command is valid for the current state
 * Part 5 Requirement: Validates commands according to state graph
 */
bool GameEngine::isValidTransition(const std::string &command) const
{
    // Part 5 Requirement: Commands are validated according to the state diagram
    if (currentState == "start" && command == "loadmap")
        return true;
    if (currentState == "map loaded" && (command == "loadmap" || command == "validatemap"))
        return true;
    if (currentState == "map validated" && command == "addplayer")
        return true;
    if (currentState == "players added" && (command == "addplayer" || command == "assigncountries"))
        return true;
    if (currentState == "assign reinforcement" && command == "issueorder")
        return true;
    if (currentState == "issue orders" && (command == "issueorder" || command == "endissueorders"))
        return true;
    if (currentState == "execute orders" && (command == "execorder" || command == "endexecorders" || command == "win"))
        return true;
    if (currentState == "win" && (command == "play" || command == "end"))
        return true;

    return false;
}

/**
 * Transitions to a new state and displays the transition
 * Part 5 Requirement: State transitions as specified in the graph
 */
void GameEngine::transition(const std::string &newState)
{
    std::cout << "Transitioning from '" << currentState
              << "' to '" << newState << "'." << std::endl;
    currentState = newState;
}

/**
 * Gets the current state
 * @return The current state as a string
 */
std::string GameEngine::getCurrentState() const
{
    return currentState;
}
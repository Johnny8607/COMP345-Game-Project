#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <string>
#include <vector>

class Map;
class Player;
class Deck;
class MapLoader;

class GameEngine
{
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

    std::vector<Player*>& getPlayers();
    Map* getMap();

    /**
     * @brief A placeholder for A2 Part 2.
     * Manually sets up the game state so the main loop can run.
     */
    void startupPhase();

    void mainGameLoop();
    void reinforcementPhase();
    void issueOrdersPhase();
    void executeOrdersPhase();
    static Player* getNeutralPlayer();            // Get (or create) neutral player
    static void setNeutralPlayer(Player* p);
    void addPlayer(Player* player);               // Register a new player
    void simulateStartup();

private:
    std::string currentState;
    Map* map;
    Deck* deck;
    // --- NEW FOR A2 ---
    static Player* neutralPlayer;        // -- Neutral player pointer
    // --- END NEW ---
    std::vector<Player*> players;
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
    
    /**
     * Helper function that distributes territories fairly among players
     */
    void distributeTerritories();
    
    /**
     * Helper function to randomly shuffle player order
     */
    void shufflePlayerOrder();

    /**
     * @brief Checks if a win condition has been met.
     */
    bool checkWinCondition();

    /**
     * @brief Removes players with no territories from the game.
     */
    void removeEliminatedPlayers();
};

#endif

#include <iostream>
#include <stdexcept> // For std::out_of_range
#include "CommandProcessing.h"
#include "GameEngine.h"
#include "Command.h" // Need Command.h for TournamentParams

void testTournament() {
    std::cout << "\n===== TESTING TOURNAMENT MODE =====\n";

    CommandProcessor cp;
    GameEngine engine; // Create GameEngine instance

    std::cout << "Enter tournament command:\n";
    
    // The CommandProcessor::readCommand() loops until 'quit' is entered.
    // It saves the tournament command and sets the state to "tournament".
    cp.readCommand(); 

    // --- LOGIC: RETRIEVE COMMAND AND RUN TOURNAMENT ---

    // 1. Check if any commands were processed
    if (cp.getCommandCount() == 0) {
        std::cout << "[INFO] Command processor exited without processing any commands." << std::endl;
        return;
    }

    // Get the index of the last command processed (size() - 1)
    size_t lastIndex = cp.getCommandCount() - 1;
    Command* cmd = nullptr;

    try {
        // CommandProcessor::getCommand returns a COPY of the command at the index.
        cmd = cp.getCommand(lastIndex); 
    } catch (const std::out_of_range& e) {
        std::cerr << "[FATAL ERROR] Could not retrieve the last command. Index was out of range." << std::endl;
        return;
    }


    // 2. Verify the last command was the successful 'tournament' command
    if (cmd && cmd->getCommand().rfind("tournament", 0) == 0) {
        
        TournamentParams params = cmd->getTournamentParams();
        
        // 3. Final validation check on retrieved parameters (should now succeed)
        if (params.games > 0 && params.maxTurns > 0 && !params.maps.empty() && !params.strategies.empty()) {
            
            std::cout << "\n--- Tournament Parameters Accepted ---\n";
            engine.runTournament(params); // Initiate the full tournament
            
        } else {
            // This case should no longer be hit if CommandProcessor::parseTournament is correct
            std::cout << "[ERROR] Tournament parameters were retrieved but were invalid (Map/Strategy list empty or G/D <= 0)." << std::endl;
        }

    } else {
        std::cout << "[INFO] Last command was not the successful 'tournament' command (State: " << cp.getState() << ")." << std::endl;
    }

    // 4. Clean up the command copy
    if (cmd) {
        delete cmd;
    }
}
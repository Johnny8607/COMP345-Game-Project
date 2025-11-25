#include <iostream>
#include "CommandProcessing.h"
#include "GameEngine.h"

void testTournament() {
    std::cout << "\n===== TESTING TOURNAMENT MODE =====\n";

    CommandProcessor cp;

    std::cout << "Enter tournament command:\n";
    cp.readCommand();

    size_t lastIndex = cp.getCommandCount() - 1;
    Command* cmd = cp.getCommand(lastIndex);

    TournamentParams params = cmd->getTournamentParams();

    GameEngine engine;
    engine.runTournament(params);

    delete cmd;
}

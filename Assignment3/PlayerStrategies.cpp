#include "PlayerStrategies.h"
#include "Player.h"
#include "Map.h"
#include "Orders.h"
#include "GameEngine.h"
#include "Hand.h"
#include "Deck.h"
#include "Cards.h"

#include <algorithm>
#include <iostream>
#include <set>
#include <limits> 
#include <cmath> 
#include <numeric>

using std::cout;
using std::endl;
using std::vector;

// Base PlayerStrategy class Implementation
PlayerStrategy::PlayerStrategy() : player(nullptr) {}
void PlayerStrategy::setPlayer(Player* p) { player = p; }
Player* PlayerStrategy::getPlayer() const { return player; }

// ============================================================
// AI Helper Functions 
// ============================================================
namespace {

Territory* strongestTerritory(Player* p) {
    auto* terrs = p->getTerritories();
    if (!terrs || terrs->empty()) return nullptr;
    return *std::max_element(
        terrs->begin(), terrs->end(),
        [](Territory* a, Territory* b) {
            // Guard against null pointers if the vector contains them
            if (!a) return true;
            if (!b) return false;
            return a->getArmies() < b->getArmies();
        }
    );
}

Territory* weakestTerritory(Player* p) {
    auto* terrs = p->getTerritories();
    if (!terrs || terrs->empty()) return nullptr;
    return *std::min_element(
        terrs->begin(), terrs->end(),
        [](Territory* a, Territory* b) {
            // Guard against null pointers
            if (!a) return false;
            if (!b) return true;
            return a->getArmies() < b->getArmies();
        }
    );
}

vector<Territory*> allAdjacentEnemies(Player* p) {
    vector<Territory*> enemies;
    std::set<Territory*> seen;

    auto* terrs = p->getTerritories();
    if (!terrs) return enemies;

    for (Territory* own : *terrs) {
        if (!own || !own->getAdjacents()) continue;
        for (Territory* adj : *own->getAdjacents()) {
            if (!adj) continue;
            // CHECK: adj->getOwner() can be nullptr (if not owned)
            Player* owner = adj->getOwner();
            if (owner != p && owner != GameEngine::getNeutralPlayer() && owner != nullptr && seen.insert(adj).second) {
                enemies.push_back(adj);
            }
        }
    }
    return enemies;
}

} // end anonymous namespace

// ============================================================
// HumanPlayerStrategy Implementation (Passive in Tournament)
// ============================================================

std::vector<Territory*> HumanPlayerStrategy::toAttack() const {
    return {}; 
}

std::vector<Territory*> HumanPlayerStrategy::toDefend() const {
    if (!player) return {};
    auto* terrs = player->getTerritories();
    if (!terrs) return {};
    return *terrs;
}

void HumanPlayerStrategy::issueOrder(GameEngine* engine) {
    if (!player) return;

    // Guaranteed non-blocking logic for tournament
    player->setReinforcementPool(0);
    player->setDoneIssuingOrders(true);
    return;
}

std::string HumanPlayerStrategy::getName() const { return "Human"; }
PlayerStrategy* HumanPlayerStrategy::clone() const { return new HumanPlayerStrategy(*this); }

// ============================================================
// AggressivePlayerStrategy Implementation
// ============================================================

std::vector<Territory*> AggressivePlayerStrategy::toAttack() const {
    if (!player) return {};
    Territory* strong = strongestTerritory(player);
    if (!strong || !strong->getAdjacents()) return {};
    vector<Territory*> result;
    for (Territory* adj : *strong->getAdjacents()) {
        if (adj && adj->getOwner() != player) result.push_back(adj);
    }
    return result;
}

std::vector<Territory*> AggressivePlayerStrategy::toDefend() const {
    if (!player) return {};
    Territory* strong = strongestTerritory(player);
    if (!strong) return {};
    return { strong };
}

void AggressivePlayerStrategy::issueOrder(GameEngine* /*engine*/) {
    if (!player) return;

    cout << "\n[Aggressive Strategy] " << player->getName() << " issuing order...\n";

    bool issued = false;

    // 1. DEPLOY PHASE: Deploy entire reinforcement pool to the strongest territory.
    int pool = player->getReinforcementPool();
    if (pool > 0) {
        Territory* strong = strongestTerritory(player);
        if (strong) {
            // *** CRITICAL CHANGE: Deploy only 1 army at a time to test stability ***
            int amount = 1; 
            if (pool < 1) amount = pool; 
            
            // Assume Deploy constructor takes Player*, Territory*, int armies
            player->addOrder(new Deploy(player, strong, amount));
            player->setReinforcementPool(pool - amount); // Decrease pool by 1
            cout << "Deploy " << amount << " to strongest territory " << strong->getName() << ". Pool left: " << player->getReinforcementPool() << "\n";
            issued = true;
            return; // Exit after one action (will be called repeatedly by GameEngine)
        } else {
             player->setReinforcementPool(0); // If cannot deploy, clear pool to exit phase
        }
    }

    // 2. ADVANCE PHASE: Attack/Fortify (if pool is 0)
    Territory* strong = strongestTerritory(player);
    if (strong && strong->getArmies() > 1) {
        
        // Find enemy to attack
        for (Territory* adj : *strong->getAdjacents()) {
            if (adj && adj->getOwner() != player) {
                int armiesToSend = strong->getArmies() / 2;
                if (armiesToSend < 1) armiesToSend = 1;
                // Assuming Advance constructor takes Player*, source*, target*, int armies
                player->addOrder(new Advance(player, strong, adj, armiesToSend));
                cout << "Advance (attack) from " << strong->getName() << " to enemy " << adj->getName() << ".\n";
                issued = true;
                player->setDoneIssuingOrders(true); 
                return;
            }
        }
        
        // Fortification move if no attack possible
        for (Territory* adj : *strong->getAdjacents()) {
            if (adj && adj->getOwner() == player && adj != strong) {
                int armiesToSend = strong->getArmies() / 2;
                if (armiesToSend < 1) armiesToSend = 1;
                 // Assuming Advance constructor takes Player*, source*, target*, int armies
                player->addOrder(new Advance(player, strong, adj, armiesToSend));
                cout << "Advance (friendly) from " << strong->getName() << " to " << adj->getName() << ".\n";
                issued = true;
                player->setDoneIssuingOrders(true);
                return;
            }
        }
    }

    // 3. Mark done
    if (!issued) {
        cout << "Aggressive player has no useful action. Marking done.\n";
        player->setDoneIssuingOrders(true);
    }
}

std::string AggressivePlayerStrategy::getName() const { return "Aggressive"; }
PlayerStrategy* AggressivePlayerStrategy::clone() const { return new AggressivePlayerStrategy(*this); }

// ============================================================
// BenevolentPlayerStrategy Implementation
// ============================================================

std::vector<Territory*> BenevolentPlayerStrategy::toAttack() const {
    return {}; // Never attacks
}

std::vector<Territory*> BenevolentPlayerStrategy::toDefend() const {
    if (!player) return {};
    auto* terrs = player->getTerritories();
    if (!terrs) return {};
    vector<Territory*> sorted = *terrs;
    std::sort(sorted.begin(), sorted.end(),
              [](Territory* a, Territory* b) {
                  // Guard against null pointers in vector
                  if (!a) return true;
                  if (!b) return false;
                  return a->getArmies() < b->getArmies(); // Weakest first
              });
    return sorted;
}

/**
 * @brief issueOrder():
 * 1. Deploy all armies to the weakest territory.
 * 2. Fortification move from strongest internal territory to weakest adjacent.
 */
void BenevolentPlayerStrategy::issueOrder(GameEngine* /*engine*/) {
    if (!player) return;

    cout << "\n[Benevolent Strategy] " << player->getName() << " issuing order...\n";

    bool issued = false;

    // 1. DEPLOY PHASE: Deploy reinforcement pool (one at a time for stability check).
    int pool = player->getReinforcementPool();
    if (pool > 0) {
        vector<Territory*> targets = toDefend();
        Territory* weak = targets.empty() ? nullptr : targets[0];

        if (weak) {
            int amount = 1; 
            if (pool < 1) amount = pool; 

            // Assume Deploy constructor takes Player*, Territory*, int armies
            player->addOrder(new Deploy(player, weak, amount));
            player->setReinforcementPool(pool - amount); // Decrease pool by 1
            cout << "Deploy " << amount << " to weakest territory " << weak->getName() << ". Pool left: " << player->getReinforcementPool() << "\n";
            issued = true;
            return; // Exit after one action
        } else {
            // If the player has a pool but no territories, clear pool to exit
            player->setReinforcementPool(0); 
            return;
        }
    }

    // 2. FORTIFICATION: Internal move (if pool is 0)
    Territory* strong = strongestTerritory(player); 
    if (strong && strong->getArmies() > 1 && strong->getAdjacents()) {
        Territory* bestTarget = nullptr;
        
        for (Territory* adj : *strong->getAdjacents()) {
            if (adj && adj->getOwner() == player && adj != strong) {
                if (!bestTarget || adj->getArmies() < bestTarget->getArmies())
                    bestTarget = adj;
            }
        }
        if (bestTarget) {
            int armiesToSend = strong->getArmies() / 2;
            if (armiesToSend < 1) armiesToSend = 1;
            // Assuming Advance constructor takes Player*, source*, target*, int armies
            player->addOrder(new Advance(player, strong, bestTarget, armiesToSend));
            cout << "Reinforcing weak territory " << bestTarget->getName() << ".\n";
            issued = true;
            player->setDoneIssuingOrders(true); // Ends turn after one fortification
            return;
        }
    }

    // 3. Mark done
    if (!issued) {
        cout << "Benevolent player has no further action. Marking done.\n";
    }
    player->setDoneIssuingOrders(true);
}

std::string BenevolentPlayerStrategy::getName() const { return "Benevolent"; }
PlayerStrategy* BenevolentPlayerStrategy::clone() const { return new BenevolentPlayerStrategy(*this); }

// ============================================================
// NeutralPlayerStrategy Implementation
// ============================================================

std::vector<Territory*> NeutralPlayerStrategy::toAttack() const { return {}; }
std::vector<Territory*> NeutralPlayerStrategy::toDefend() const { return {}; }
void NeutralPlayerStrategy::issueOrder(GameEngine* /*engine*/) {
    if (!player) return;
    cout << "\n[Neutral Strategy] " << player->getName()
         << " does nothing this round.\n";
    player->setDoneIssuingOrders(true);
}

std::string NeutralPlayerStrategy::getName() const { return "Neutral"; }
PlayerStrategy* NeutralPlayerStrategy::clone() const { return new NeutralPlayerStrategy(*this); }

// ============================================================
// CheaterPlayerStrategy Implementation
// ============================================================

std::vector<Territory*> CheaterPlayerStrategy::toAttack() const {
    if (!player) return {};
    return allAdjacentEnemies(player);
}

std::vector<Territory*> CheaterPlayerStrategy::toDefend() const {
    if (!player) return {};
    auto* terrs = player->getTerritories();
    if (!terrs) return {};
    return *terrs;
}

/**
 * @brief issueOrder():
 * Cheater automatically conquers all adjacent enemy territories once per turn.
 */
void CheaterPlayerStrategy::issueOrder(GameEngine* /*engine*/) {
    if (!player) return;

    cout << "\n[Cheater Strategy] " << player->getName()
         << " auto-conquers adjacent territories (once per turn).\n";

    if (player->hasPlayedCardThisRound) {
        cout << "Cheater already performed cheating action this turn. Done.\n";
        player->setDoneIssuingOrders(true);
        return;
    }

    auto* terrs = player->getTerritories();
    if (!terrs || terrs->empty()) {
        player->setDoneIssuingOrders(true);
        return;
    }

    // --- CONQUER LOGIC ---
    // Deploy remaining reinforcement pool
    int pool = player->getReinforcementPool();
    if (pool > 0) {
        Territory* strong = strongestTerritory(player);
        if (strong) {
             player->addOrder(new Deploy(player, strong, pool));
             player->setReinforcementPool(0);
             cout << "Deploying remaining " << pool << " armies to " << strong->getName() << ".\n";
        }
    }

    // Mark as "used cheat power" for this turn and done issuing orders
    player->hasPlayedCardThisRound = true;
    player->setDoneIssuingOrders(true);
}

std::string CheaterPlayerStrategy::getName() const { return "Cheater"; }
PlayerStrategy* CheaterPlayerStrategy::clone() const { return new CheaterPlayerStrategy(*this); }
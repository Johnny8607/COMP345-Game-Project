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

using std::cout;
using std::endl;
using std::vector;

// Base PlayerStrategy class
PlayerStrategy::PlayerStrategy() : player(nullptr) {}

void PlayerStrategy::setPlayer(Player* p) { player = p; }

//Retrieve the associated Player
Player* PlayerStrategy::getPlayer() const { return player; }

// Helper functions 
namespace {

/**
 * @brief Returns the strongest (max armies) territory owned by player.
 * @param p The player whose territory strength is evaluated.
 */
Territory* strongestTerritory(Player* p) {
    auto* terrs = p->getTerritories();
    if (!terrs || terrs->empty()) return nullptr;
    return *std::max_element(
        terrs->begin(), terrs->end(),
        [](Territory* a, Territory* b) {
            return a->getArmies() < b->getArmies();
        }
    );
}

/**
 * @brief Returns the weakest (min armies) territory owned by player.
 * @param p The player whose territory weakness is evaluated.
 */
Territory* weakestTerritory(Player* p) {
    auto* terrs = p->getTerritories();
    if (!terrs || terrs->empty()) return nullptr;
    return *std::min_element(
        terrs->begin(), terrs->end(),
        [](Territory* a, Territory* b) {
            return a->getArmies() < b->getArmies();
        }
    );
}

/**
 * @brief Returns all enemy territories adjacent to any of the player's territories.
 * @param p The player whose enemies are being scanned.
 */
vector<Territory*> allAdjacentEnemies(Player* p) {
    vector<Territory*> enemies;
    std::set<Territory*> seen;

    auto* terrs = p->getTerritories();
    if (!terrs) return enemies;

    for (Territory* own : *terrs) {
        if (!own || !own->getAdjacents()) continue;
        for (Territory* adj : *own->getAdjacents()) {
            if (!adj) continue;
            if (adj->getOwner() != p && seen.insert(adj).second) {
                enemies.push_back(adj);
            }
        }
    }
    return enemies;
}

} 

// HumanPlayerStrategy 

std::vector<Territory*> HumanPlayerStrategy::toAttack() const {
    if (!player) return {};
    // For simplicity: return all adjacent enemy territories
    return allAdjacentEnemies(player);
}

std::vector<Territory*> HumanPlayerStrategy::toDefend() const {
    if (!player) return {};
    // For simplicity: return all owned territories
    auto* terrs = player->getTerritories();
    if (!terrs) return {};
    return *terrs;
}

void HumanPlayerStrategy::issueOrder(GameEngine* engine) {
    if (!player) return;

    cout << "\n[Human Strategy] Player " << player->getName()
         << " issued one order.\n";

    // Check if human has anything to do. If not: mark done
    if (player->getReinforcementPool() <= 0 &&
        (!player->getTerritories() || player->getTerritories()->empty()) &&
        player->getHand()->getAllCards().empty()) {
        cout << "No actions available. Marking done.\n";
        player->setDoneIssuingOrders(true);
        return;
    }

    // Present options
    cout << "Choose action:\n";
    cout << "  1) Deploy\n";
    cout << "  2) Advance\n";
    cout << "  3) Play a card\n";
    cout << "  4) Finish issuing orders\n";
    cout << "Enter choice (1-4): ";

    int choice = 0;
    std::cin >> choice;
    // Clear input buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    bool issued = false;

    if (choice == 1) {
        // DEPLOY
        if (player->getReinforcementPool() <= 0) {
            cout << "No reinforcements left.\n";
        } else {
            auto* terrs = player->getTerritories();
            if (!terrs || terrs->empty()) {
                cout << "You own no territories.\n";
            } else {
                cout << "Select territory index to deploy:\n";
                for (size_t i = 0; i < terrs->size(); ++i) {
                    cout << "  " << i << ") " << (*terrs)[i]->getName()
                         << " (armies: " << (*terrs)[i]->getArmies() << ")\n";
                }
                cout << "Index: ";
                size_t idx;
                std::cin >> idx;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (idx < terrs->size()) {
                    int maxArmies = player->getReinforcementPool();
                    cout << "Enter armies to deploy (1-" << maxArmies << "): ";
                    int n;
                    std::cin >> n;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    n = std::max(1, std::min(n, maxArmies));
                    player->addOrder(new Deploy(player, (*terrs)[idx], n));
                    player->setReinforcementPool(maxArmies - n);
                    cout << "Queued DEPLOY.\n";
                    issued = true;
                } else {
                    cout << "Invalid index.\n";
                }
            }
        }
    } else if (choice == 2) {
        // ADVANCE
        auto* terrs = player->getTerritories();
        if (!terrs || terrs->empty()) {
            cout << "You own no territories.\n";
        } else {
            cout << "Select source territory (by index):\n";
            for (size_t i = 0; i < terrs->size(); ++i) {
                cout << "  " << i << ") " << (*terrs)[i]->getName()
                     << " (armies: " << (*terrs)[i]->getArmies() << ")\n";
            }
            cout << "Source index: ";
            size_t sIdx;
            std::cin >> sIdx;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (sIdx < terrs->size()) {
                Territory* src = (*terrs)[sIdx];
                auto* adj = src->getAdjacents();
                if (!adj || adj->empty()) {
                    cout << "No adjacent territories.\n";
                } else {
                    cout << "Select target territory among adjacents:\n";
                    for (size_t i = 0; i < adj->size(); ++i) {
                        cout << "  " << i << ") " << (*adj)[i]->getName()
                             << " (owner: "
                             << ((*adj)[i]->getOwner()
                                 ? (*adj)[i]->getOwner()->getName()
                                 : "none")
                             << ", armies: " << (*adj)[i]->getArmies()
                             << ")\n";
                    }
                    cout << "Target index: ";
                    size_t tIdx;
                    std::cin >> tIdx;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    if (tIdx < adj->size()) {
                        int maxArmies = src->getArmies();
                        if (maxArmies <= 0) {
                            cout << "No armies to move.\n";
                        } else {
                            cout << "Enter armies to advance (1-" << maxArmies << "): ";
                            int n;
                            std::cin >> n;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            n = std::max(1, std::min(n, maxArmies));
                            player->addOrder(new Advance(player, src, (*adj)[tIdx], n));
                            cout << "Queued ADVANCE.\n";
                            issued = true;
                        }
                    } else {
                        cout << "Invalid index.\n";
                    }
                }
            } else {
                cout << "Invalid index.\n";
            }
        }
    } else if (choice == 3) {
        // PLAY CARD
        auto& cards = player->getHand()->getAllCards();
        if (cards.empty()) {
            cout << "No cards to play.\n";
        } else if (player->hasPlayedCardThisRound) {
            cout << "You already played a card this round.\n";
        } else {
            cout << "Cards in hand:\n";
            for (size_t i = 0; i < cards.size(); ++i) {
                cout << "  " << i << ") " << cards[i]->toString() << "\n";
            }
            cout << "Choose card index: ";
            size_t cIdx;
            std::cin >> cIdx;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (cIdx < cards.size()) {
                Card* card = cards[cIdx];
                Order* o = card->play(player);
                if (o) player->addOrder(o);
                player->getHand()->removeCard(card);
                // Optionally return card to deck via engine->getDeck()
                if (engine && engine->getDeck()) {
                    engine->getDeck()->addCard(card);
                }
                player->hasPlayedCardThisRound = true;
                cout << "Card played.\n";
                issued = true;
            } else {
                cout << "Invalid card index.\n";
            }
        }
    } else if (choice == 4) {
        cout << "Player chose to stop issuing orders.\n";
        player->setDoneIssuingOrders(true);
        return;
    } else {
        cout << "Invalid choice.\n";
    }

    if (!issued) {
        // if we did nothing useful this call and have nothing to do, mark done and end issuing
        if (player->getReinforcementPool() <= 0 &&
            (!player->getTerritories() || player->getTerritories()->empty()) &&
            player->getHand()->getAllCards().empty()) {
            player->setDoneIssuingOrders(true);
        }
    }
}

std::string HumanPlayerStrategy::getName() const { return "Human"; }
PlayerStrategy* HumanPlayerStrategy::clone() const { return new HumanPlayerStrategy(*this); }


//AggressivePlayerStrategy 
/**
 * @brief toAttack():
 * Returns all enemy territories adjacent to the strongest territory.
 */

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

/**
 * @brief toDefend():
 * Only defend the strongest territory.
 */
std::vector<Territory*> AggressivePlayerStrategy::toDefend() const {
    if (!player) return {};
    Territory* strong = strongestTerritory(player);
    if (!strong) return {};
    return { strong };
}

/**
 * @brief issueOrder():
 * 1. Deploy all armies (in chunks of up to 3) onto strongest territory.
 * 2. Attack neighbors with a strong advance.
 * 3. If no enemy adjacent, advance internally.
 * 4. If no moves available, end turn.
 */
void AggressivePlayerStrategy::issueOrder(GameEngine* /*engine*/) {
    if (!player) return;

    cout << "\n[Aggressive Strategy] " << player->getName() << " issuing order...\n";

    bool issued = false;

    // 1. Deploy all reinforcements to the strongest country (in chunks of up to 3)
    int pool = player->getReinforcementPool();
    if (pool > 0) {
        Territory* strong = strongestTerritory(player);
        if (strong) {
            int amount = std::min(3, pool);
            player->addOrder(new Deploy(player, strong, amount));
            player->setReinforcementPool(pool - amount);
            cout << "Deploy " << amount << " to strongest territory " << strong->getName() << ".\n";
            issued = true;
            // keep DoneIssuingOrders=false so GameEngine calls again if pool remains
            return;
        }
    }

    // 2. Attack from strongest territory to adjacent enemy
    Territory* strong = strongestTerritory(player);
    if (strong && strong->getAdjacents() && strong->getArmies() > 1) {
        for (Territory* adj : *strong->getAdjacents()) {
            if (adj && adj->getOwner() != player) {
                int armiesToSend = strong->getArmies() / 2;
                if (armiesToSend < 1) armiesToSend = 1;
                player->addOrder(new Advance(player, strong, adj, armiesToSend));
                cout << "Advance from " << strong->getName() << " to enemy "
                     << adj->getName() << " with " << armiesToSend << " armies.\n";
                issued = true;
                player->setDoneIssuingOrders(true); // one attack per round
                return;
            }
        }
    }

    // 3. If no enemy adjacent, try friendly move (not required, but safe)
    if (strong && strong->getAdjacents() && strong->getArmies() > 1) {
        for (Territory* adj : *strong->getAdjacents()) {
            if (adj && adj->getOwner() == player) {
                int armiesToSend = strong->getArmies() / 2;
                if (armiesToSend < 1) armiesToSend = 1;
                player->addOrder(new Advance(player, strong, adj, armiesToSend));
                cout << "Advance (friendly) from " << strong->getName()
                     << " to " << adj->getName() << ".\n";
                issued = true;
                player->setDoneIssuingOrders(true);
                return;
            }
        }
    }

    // 4.nothing to do

    if (!issued) {
        cout << "Aggressive player has no useful action. Marking done.\n";
        player->setDoneIssuingOrders(true);
    }
}

std::string AggressivePlayerStrategy::getName() const { return "Aggressive"; }
PlayerStrategy* AggressivePlayerStrategy::clone() const { return new AggressivePlayerStrategy(*this); }


//  BenevolentPlayerStrategy 

/**
 * @brief toAttack():
 * Benevolent player NEVER attacks.
 */
std::vector<Territory*> BenevolentPlayerStrategy::toAttack() const {
    return {};
}

/**
 * @brief toDefend():
 * Returns owned territories sorted from weakest to strongest.
 */
std::vector<Territory*> BenevolentPlayerStrategy::toDefend() const {
    if (!player) return {};
    auto* terrs = player->getTerritories();
    if (!terrs) return {};
    vector<Territory*> sorted = *terrs;
    std::sort(sorted.begin(), sorted.end(),
              [](Territory* a, Territory* b) {
                  return a->getArmies() < b->getArmies();
              });
    return sorted;
}

/**
 * @brief issueOrder():
 * 1. Deploy to weakest territory.
 * 2. Well-mannered internal move from strong to weakest adjacent.
 * 3. Does not play harmful cards.
 */
void BenevolentPlayerStrategy::issueOrder(GameEngine* /*engine*/) {
    if (!player) return;

    cout << "\n[Benevolent Strategy] " << player->getName() << " issuing order...\n";

    bool issued = false;

    // 1. Deploy to weakest countries
    int pool = player->getReinforcementPool();
    if (pool > 0) {
        Territory* weak = weakestTerritory(player);
        if (weak) {
            int amount = std::min(3, pool);
            player->addOrder(new Deploy(player, weak, amount));
            player->setReinforcementPool(pool - amount);
            cout << "Deploy " << amount << " to weakest territory " << weak->getName() << ".\n";
            issued = true;
            return; // keep issuing while pool left
        }
    }

    // 2. Internal move from stronger to weaker territory (never to enemy)
    auto* terrs = player->getTerritories();
    if (terrs && !terrs->empty()) {
        // pick a strong territory with enough armies
        Territory* strong = nullptr;
        for (Territory* t : *terrs) {
            if (!t) continue;
            if (!strong || t->getArmies() > strong->getArmies())
                strong = t;
        }

        if (strong && strong->getArmies() > 1 && strong->getAdjacents()) {
            Territory* bestTarget = nullptr;
            for (Territory* adj : *strong->getAdjacents()) {
                if (adj && adj->getOwner() == player) {
                    if (!bestTarget || adj->getArmies() < bestTarget->getArmies())
                        bestTarget = adj;
                }
            }
            if (bestTarget) {
                int armiesToSend = strong->getArmies() / 2;
                if (armiesToSend < 1) armiesToSend = 1;
                player->addOrder(new Advance(player, strong, bestTarget, armiesToSend));
                cout << "Reinforcing weak territory " << bestTarget->getName()
                     << " from " << strong->getName() << " with "
                     << armiesToSend << " armies.\n";
                issued = true;
                player->setDoneIssuingOrders(true);
                return;
            }
        }
    }

    // 3. Cards: to fully guarantee non-harmful use
    if (!issued) {
        cout << "Benevolent player has no peaceful action. Marking done.\n";
        player->setDoneIssuingOrders(true);
    }
}

std::string BenevolentPlayerStrategy::getName() const { return "Benevolent"; }
PlayerStrategy* BenevolentPlayerStrategy::clone() const { return new BenevolentPlayerStrategy(*this); }

//NeutralPlayerStrategy

/**
 * @brief toAttack():
 * Neutral NEVER attacks.
 */

std::vector<Territory*> NeutralPlayerStrategy::toAttack() const {
    return {};
}

/**
 * @brief toDefend():
 * Neutral does not care about defending.
 */
std::vector<Territory*> NeutralPlayerStrategy::toDefend() const {
    return {};
}

/**
 * @brief issueOrder():
 * Neutral never issues orders.
 */
void NeutralPlayerStrategy::issueOrder(GameEngine* /*engine*/) {
    if (!player) return;
    cout << "\n[Neutral Strategy] " << player->getName()
         << " does nothing this round.\n";
    player->setDoneIssuingOrders(true);
}

std::string NeutralPlayerStrategy::getName() const { return "Neutral"; }
PlayerStrategy* NeutralPlayerStrategy::clone() const { return new NeutralPlayerStrategy(*this); }

//CheaterPlayerStrategy
/**
 * @brief toAttack():
 * Cheater attacks all adjacent enemy territories (auto-conquers them).
 */
std::vector<Territory*> CheaterPlayerStrategy::toAttack() const {
    if (!player) return {};
    return allAdjacentEnemies(player);
}

/**
 * @brief toDefend():
 * Returns all owned territories (no special rule).
 */
std::vector<Territory*> CheaterPlayerStrategy::toDefend() const {
    if (!player) return {};
    auto* terrs = player->getTerritories();
    if (!terrs) return {};
    return *terrs;
}

/**
 * @brief issueOrder():
 * Cheater automatically conquers all adjacent enemy territories once per turn.
 *
 * re-use player->hasPlayedCardThisRound as a "cheater-used" flag.
 *
 * Behavior:
 *  - Find all adjacent enemy territories
 *  - Remove them from their owner
 *  - Give them to Cheater
 *  - Mark as conquered
 */
void CheaterPlayerStrategy::issueOrder(GameEngine* /*engine*/) {
    if (!player) return;

    cout << "\n[Cheater Strategy] " << player->getName()
         << " auto-conquers adjacent territories (once per turn).\n";

    // Use hasPlayedCardThisRound as a per-turn flag to enforce "once per turn"
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

    std::set<Territory*> toConquer;

    // Collect all adjacent enemy territories
    for (Territory* own : *terrs) {
        if (!own || !own->getAdjacents()) continue;
        for (Territory* adj : *own->getAdjacents()) {
            if (!adj) continue;
            if (adj->getOwner() != player) {
                toConquer.insert(adj);
            }
        }
    }

    if (toConquer.empty()) {
        cout << "No adjacent enemy territories to cheat-conquer.\n";
        player->setDoneIssuingOrders(true);
        return;
    }

     // Conquer
    for (Territory* t : toConquer) {
        Player* oldOwner = t->getOwner();
        if (oldOwner && oldOwner != player) {
            // remove from old owner's list
            auto* oldTerrs = oldOwner->getTerritories();
            if (oldTerrs) {
                oldTerrs->erase(
                    std::remove(oldTerrs->begin(), oldTerrs->end(), t),
                    oldTerrs->end()
                );
            }
        }
        t->setOwner(player);
        player->addTerritory(t);
        player->setConqueredTerritory(true);
        cout << "Cheater instantly conquers " << t->getName() << ".\n";
    }

    // Mark as "used cheat power" for this turn
    player->hasPlayedCardThisRound = true;
    player->setDoneIssuingOrders(true);
}

std::string CheaterPlayerStrategy::getName() const { return "Cheater"; }
PlayerStrategy* CheaterPlayerStrategy::clone() const { return new CheaterPlayerStrategy(*this); }

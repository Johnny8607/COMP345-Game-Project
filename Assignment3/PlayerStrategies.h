#pragma once

#include <vector>
#include <string>

// Forward declarations to avoid circular includes
class Player;
class Territory;
class GameEngine;


class PlayerStrategy {
protected:
    Player* player;    // the Player using this strategy

public:
    PlayerStrategy();
    virtual ~PlayerStrategy() = default;

    void setPlayer(Player* p);
    Player* getPlayer() const;

    // Each strategy provides its own behavior
    virtual std::vector<Territory*> toAttack() const = 0;
    virtual std::vector<Territory*> toDefend() const = 0;
    virtual void issueOrder(GameEngine* engine) = 0;
    virtual std::string getName() const = 0;

    // For copying Players safely
    virtual PlayerStrategy* clone() const = 0;
};

// Strategy for a human-controlled player

class HumanPlayerStrategy : public PlayerStrategy {
public:
    std::vector<Territory*> toAttack() const override;
    std::vector<Territory*> toDefend() const override;
    void issueOrder(GameEngine* engine) override;
    std::string getName() const override;
    PlayerStrategy* clone() const override;
};

//Strategy for an aggressive computer player.
class AggressivePlayerStrategy : public PlayerStrategy {
public:
    std::vector<Territory*> toAttack() const override;
    std::vector<Territory*> toDefend() const override;
    void issueOrder(GameEngine* engine) override;
    std::string getName() const override;
    PlayerStrategy* clone() const override;
};

//Strategy for a benevolent computer player
class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    std::vector<Territory*> toAttack() const override;
    std::vector<Territory*> toDefend() const override;
    void issueOrder(GameEngine* engine) override;
    std::string getName() const override;
    PlayerStrategy* clone() const override;
};

//Strategy for a neutral computer player
class NeutralPlayerStrategy : public PlayerStrategy {
public:
    std::vector<Territory*> toAttack() const override;
    std::vector<Territory*> toDefend() const override;
    void issueOrder(GameEngine* engine) override;
    std::string getName() const override;
    PlayerStrategy* clone() const override;
};

//Strategy for a cheater computer player
class CheaterPlayerStrategy : public PlayerStrategy {
public:
    std::vector<Territory*> toAttack() const override;
    std::vector<Territory*> toDefend() const override;
    void issueOrder(GameEngine* engine) override;
    std::string getName() const override;
    PlayerStrategy* clone() const override;
};

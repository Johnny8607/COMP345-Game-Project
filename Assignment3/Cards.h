#ifndef CARDS_H
#define CARDS_H

#include <iostream>

class Player;
class Order;

enum class CardType {
    Bomb, 
    Reinforcement, 
    Blockade, 
    Airlift, 
    Diplomacy
};

class Card {
    private: 
        CardType* type;

    public:
        // default constructor
        Card(CardType type);
        // copy constructor
        Card(const Card& other);
        // assignment operator
        Card& operator=(const Card& other);
        // desctructor
        ~Card();

        CardType getType() const;
        std::string toString();

        Order* play(Player* player);
    };

    // stream insertion operator for CardType
    std::ostream& operator<<(std::ostream& os, const CardType type);

    // stream insertion operator for Card
    std::ostream& operator<<(std::ostream& os, const Card& card);

#endif

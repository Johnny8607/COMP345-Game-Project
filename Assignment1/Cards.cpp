#include "Cards.h"

// default constructor
Card::Card(CardType type) {
    this->type = new CardType(type);
};

// copy constructor
Card::Card(const Card& other) {
    this->type = new CardType(*other.type);
}

// assignment operator
Card& Card::operator=(const Card& other) {
    if (this != &other) {

        delete type;
        type = new CardType(*other.type);
    }
    return *this;
}

// destructor
Card::~Card() {
    delete type;
}

CardType Card::getType() const {
    return *type;
}

void Card::play() {
    std::cout << "playing card => " << getType() << std::endl;
    // TODO creates an order and adds it to the player list
}

// stream insertion operator for CardType
std::ostream &operator<<(std::ostream &os, const CardType type)
{
    switch (type) {
        case CardType::Bomb: os << "Bomb"; break;
        case CardType::Reinforcement: os << "Reinforcement"; break;
        case CardType::Blockade: os << "Blockade"; break;
        case CardType::Airlift: os << "Airlift"; break;
        case CardType::Diplomacy: os << "Diplomacy"; break;
    }
    return os;
}

// stream insertion operator for Card
std::ostream &operator<<(std::ostream &os, const Card &card)
{
    os << "Card of type: " << card.getType();
    return os;
}

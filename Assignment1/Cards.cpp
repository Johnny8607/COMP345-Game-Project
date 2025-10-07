#include "Cards.h"
#include "Orders.h"
#include "Player.h"

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

std::string Card::toString() {
    switch (this->getType()) {
        case CardType::Bomb: return "bomb";
        case CardType::Reinforcement: return "deploy";
        case CardType::Blockade: return "blockade";
        case CardType::Airlift: return "airlift"; 
        case CardType::Diplomacy: return "negotiate";
        default: return "unknown";
    }
}

void Card::play(Player* player) {
    std::cout << "playing card => " << getType() << std::endl;
    
    OrderFactory factory;
    Order* order = factory.createOrder(this->toString());

    if (order) {
        player->addOrder(order);
    }
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

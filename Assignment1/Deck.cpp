#include "Deck.h"
#include <random>
#include <algorithm>

// default constructor
Deck::Deck() {
    cards = new std::vector<Card*>();
}

// copy constructor
Deck::Deck(const Deck& other) {
    cards = new std::vector<Card*>();
    for (Card* card : *other.cards) {
        cards->push_back(new Card(*card));
    }
}

// assignment constructor
Deck& Deck::operator=(const Deck& other) {
    if (this != &other) {
        // delete previous deck from memory
        for (Card* card : *cards) {
            delete card;
        }
        delete cards;

        cards = new std::vector<Card*>();
        for (Card* card : *other.cards) {
            cards->push_back(new Card(*card));
        }
    }
    return *this;
}

// desctructor
Deck::~Deck() {
    for (Card* card : *cards) {
        delete card;
    }
    delete cards;
}


int Deck::size() const {
    return cards->size();
}

void Deck::addCard(Card *card) {
    cards->push_back(card);
}

Card *Deck::draw() {
    if (cards->empty()) {
        return nullptr;
    }

    // random device generator used to get a random number
    std::random_device random_device;
    std::mt19937 gen(random_device());
    std::uniform_int_distribution<> dis(0, cards->size() - 1);
    int randomNumber = dis(gen);

    // use random number to draw a random card
    Card* cardDrawn = cards->at(randomNumber);
    cards->erase(cards->begin() + randomNumber);

    return cardDrawn;
}

const std::vector<Card*>& Deck::getAllCards() const {
    return *cards;
}

std::ostream &operator<<(std::ostream &os, const Deck &deck) {
   os << "Deck contains " << deck.size() << " cards:\n";
   for (Card* card : deck.getAllCards()) {
    os << "-" << *card << "\n" ; 
   }

   return os;
}

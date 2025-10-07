#include "Hand.h"
#include <algorithm>
#include <iostream>

// default constructor
Hand::Hand() {
    cards = new std::vector<Card*>();
}

// copy constructor
Hand::Hand(const Hand& other) {
    cards = new std::vector<Card*>();
    for (auto card : *other.cards) {
        cards->push_back(new Card(*card));
    }
}

// assignment constructor
Hand& Hand::operator=(const Hand& other) {
    if (this != &other) {
        for (auto card : *cards) {
            delete card;
        }
        delete cards;

        cards = new std::vector<Card*>();
        for (auto card: *other.cards) {
            cards->push_back(new Card(*card));
        }
    }
    return *this;
}

// destructor
Hand::~Hand() {
    for (auto card: *cards) {
        delete card;
    }
    delete cards;
}

int Hand::size() const {
    return cards->size();
}

void Hand::addCard(Card* card) {
    cards->push_back(card);
}

void Hand::removeCard(Card * card) {
    auto cardToRemove = std::find(cards->begin(), cards->end(), card);
    if (cardToRemove != cards->end()) {
        cards->erase(cardToRemove);
    }
}

void Hand::playCard(Deck *deck) {
    if (cards->empty()) {
        return;
    }

    // random device generator used to get a random number
    std::random_device random_device;
    std::mt19937 gen(random_device());
    std::uniform_int_distribution<> dis(0, cards->size() - 1);
    int randomNumber = dis(gen);

    // use random number to play a random card
    Card* card = cards->at(randomNumber);
    card->play();
    removeCard(card);
    deck->addCard(card);
}

std::vector<Card*>& Hand::getAllCards() const {
    return *cards;
}

std::ostream & operator<<(std::ostream & os, const Hand & hand) {
   os << "Cards in hand " << hand.size() << "\n";
   for (Card* card : hand.getAllCards()) {
    os << "-" << *card << "\n" ; 
   }
   return os;
}

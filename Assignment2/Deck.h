#ifndef DECK_H
#define DECK_H

#include <iostream>
#include <vector>

class Card;

class Deck {
private:
    std::vector<Card*>* cards;

public:
    // default constructor
    Deck();
    // copy constructor
    Deck(const Deck& other);
    // assignment constructor
    Deck& operator=(const Deck& other);
    // desctructor
    ~Deck();

    int size() const;
    void addCard(Card* card);
    Card* draw();
    const std::vector<Card*>& getAllCards() const;
};

// stream insertion operator for CardType
std::ostream& operator<<(std::ostream& os, const Deck& deck);


#endif

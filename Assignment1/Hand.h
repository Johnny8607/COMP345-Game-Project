#ifndef HAND_H
#define HAND_H

#include <iostream>
#include <random>
#include <vector>
#include "Cards.h"
#include "Deck.h"

class Hand {
    private:
        std::vector<Card*>* cards;

    public:
        // default constructor
        Hand();
        // copy constructor
        Hand(const Hand& other);
        // assignment constructor
        Hand& operator=(const Hand& other);
        // descructor
        ~Hand();

        int size() const;
        void addCard(Card* card);
        void removeCard(Card* card);
        void playCard(Deck* deck);
        std::vector<Card*>& getAllCards() const;
    };

// stream insertion operator
std::ostream& operator<<(std:: ostream& os, const Hand& hand);
    
#endif

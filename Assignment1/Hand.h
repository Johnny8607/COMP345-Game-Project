#ifndef HAND_H
#define HAND_H

#include <iostream>
#include <random>
#include <vector>

class Card;

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
        std::vector<Card*>& getAllCards() const;
    };

// stream insertion operator
std::ostream& operator<<(std:: ostream& os, const Hand& hand);
    
#endif

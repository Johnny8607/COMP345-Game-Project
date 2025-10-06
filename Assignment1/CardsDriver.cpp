#include "Cards.h"
#include "Deck.h"
#include "Hand.h"
#include <iostream>
using namespace std;

void testCards() {
    // create a deck of cards
    Deck deck;
    cout << "Generating deck..." << endl;

    deck.addCard(new Card(CardType::Bomb));
    deck.addCard(new Card(CardType::Reinforcement));
    deck.addCard(new Card(CardType::Blockade));
    deck.addCard(new Card(CardType::Airlift));
    deck.addCard(new Card(CardType::Diplomacy));

    cout << "Initial deck:\n" << deck << endl;

    // create a Hand object and fill it
    cout << "Generating hand..." << endl;
    Hand hand;

    while(deck.size() > 0) {
        Card * drawnCard = deck.draw();
        hand.addCard(drawnCard);
        cout << "Drew " << *drawnCard << endl;
    }

    cout << "\nDeck after drawing:\n" << deck << endl;
    cout << "Hand after drawing from deck:\n" << hand << endl;


    // play all the cards
    cout << "Playing all the cards..." << endl;
    while(hand.size() > 0) {
        hand.playCard(&deck);
    }
    
    cout << "\nDeck after playing all cards:\n" << deck << endl;
    cout << "Hand after playing all cards:\n" << hand << endl;
}

int main() {
    testCards();
    return 0;
}

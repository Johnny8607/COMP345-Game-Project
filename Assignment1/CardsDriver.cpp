#include "Cards.h"
#include "Deck.h"
#include "Hand.h"
#include "Player.h"
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
    Player player("David");

    while(deck.size() > 0) {
        Card * drawnCard = deck.draw();
        player.getHand()->addCard(drawnCard);
        cout << "Drew " << *drawnCard << endl;
    }

    cout << "\nDeck after drawing:\n" << deck << endl;
    cout << "Hand after drawing from deck:\n" << *player.getHand() << endl;


    // play all the cards
    cout << "Playing all the cards..." << endl;
    while(player.getHand()->size() > 0) {
        player.playCard(&deck);
    }
    
    cout << "\nDeck after playing all cards:\n" << deck << endl;
    cout << "Hand after playing all cards:\n" << *player.getHand() << endl;

    cout << "Player orders :\n" << *player.getOrders() << endl;
}

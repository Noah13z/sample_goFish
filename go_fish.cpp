// FILE: card_demo.cpp
// This is a small demonstration program showing how the Card and Deck classes are used.
#include <iostream>    // Provides cout and cin
#include <fstream>
#include <cstdlib>     // Provides EXIT_SUCCESS
#include "card.h"
#include "player.h"
#include "deck.h"

using namespace std;

// PROTOTYPES for functions used by this demonstration program:
void dealHand(Deck &d, Player &p, int numCards);
int playGame(Deck &d, Player &p1, Player &p2, ofstream& myfile);
void printHands(Deck &d, Player &p1, Player &p2, ofstream& myfile);

int main( )
{
    ofstream myfile;
    myfile.open ("gofish_results.txt");

    int numCards = 5;

    Player p1("Joe");
    Player p2("Don");
    
    Deck d;  //create a deck of cards
    d.shuffle(); //shuffle them

    dealHand(d, p1, numCards);//deal 5 cards to player1
    dealHand(d, p2, numCards);//deal 5 cards to player2

    //prints hand
    myfile << p1.getName() <<" has : " << p1.showHand() << endl;
    myfile << p2.getName() <<" has : " << p2.showHand() << "\n" << endl;

    int turns = playGame(d, p1, p2, myfile);//function call to play the game through

    //prints books
    myfile << p1.getName() << "'s book has : " << p1.showBooks() << endl;
    myfile << p2.getName() << "'s books has : " << p2.showBooks() << endl;

    //determine winner
    if (p1.getBookSize() > p2.getBookSize())
        myfile<<p1.getName()<<" won in "<< turns << " turns!\n";
    else if (p2.getBookSize() > p1.getBookSize())
        myfile<<p2.getName()<<" won in "<< turns << " turns!\n";
    else
        myfile<<"It's a Tie - "<< turns << " turns\n";

    myfile.close();
    return EXIT_SUCCESS;  
}

//deal cards to players
void dealHand(Deck &d, Player &p, int numCards)
{
   for (int i=0; i < numCards; i++){
       Card n = d.dealCard();
       if (n.getRank() != 0)
         p.addCard(n);
   }
}

//play the game through and return amount of turns
int playGame(Deck &d, Player &p1, Player &p2,ofstream& myfile)
{
    int c = 0;//turns counter
    //while loop to go through turns
    while (true)
    {
        //exit case
        if (d.size()==0 && p1.getHandSize() == 0 && p2.getHandSize() == 0)
            break;

        //if the current player has no cards, give a card and skip turn
        if (p1.getHandSize() == 0)
            if (d.size() != 0){
                p1.addCard(d.dealCard());
                printHands(d,p1,p2,myfile);
                continue;
            }

        //p1 asks p2 for card
        Card crd = p1.chooseCardFromHand();
        myfile << p1.getName() << " asks - Do you have a " << crd.rankString(crd.getRank()) << "?" << endl;

        //if p2 has the card
        if (p2.getHandSize() != 0 && p2.cardInHand(crd)) {
            myfile << p2.getName() << " says - Yes. I have a " << crd.rankString(crd.getRank()) << "." << endl;
            //remove card from p2 and give it to p1 to book
            p1.addCard(p2.removeCardFromHand(crd));
            myfile << p1.getName() << " books the " << crd.rankString(crd.getRank()) << endl;
        }

        //if p2 doesnt, go fish
        else {
            myfile << p2.getName() << " says - Go Fish" << endl;
            //if deck is empty, p1 can draw no cards
            if (d.size() == 0) {
                myfile << "Deck is empty" << endl;
            }
            //if deck is not empty
            else {
                Card tmp = d.dealCard();//draw a card
                //if new card can make a book
                if (p1.cardInHand(tmp)) {
                    p1.addCard(tmp);//add it to p1 hand
                    myfile << p1.getName() << " books the " << tmp.rankString(tmp.getRank()) << ".." << endl;
                }
                //if new card is unique
                else {
                    p1.addCard(tmp);//add it to p1 hand
                    myfile << p1.getName() << " draws " << tmp.rankString(tmp.getRank()) << endl;
                }
            }
        }

        printHands(d,p1,p2,myfile);//print out the hands of each player

        //check if player1 are without cards, give them a card and print again
        if (p1.getHandSize() == 0)
            if (d.size() != 0) {
                p1.addCard(d.dealCard());
                printHands(d,p1,p2,myfile);
            }

        //switch players (p1 is now p2, p2 is now p1)
        Player tp = p2;
        p2 = p1;
        p1 = tp;
        c++;//increment turns
    }
    return c;
}

//function to print out the cards of each player
void printHands(Deck &d, Player &p1, Player &p2, ofstream& myfile)
{
    myfile << endl;
    myfile << p1.getName() << " has : " << p1.showHand() << endl;
    myfile << p2.getName() << " has : " << p2.showHand() << endl;
}
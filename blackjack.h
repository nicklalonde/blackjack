#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define NUM_CARDS 52
#define NUM_PLAYERS 4

#define FOLD 0
#define CALL 1
#define RAISE 2
#define CHECK 3

typedef struct {
    int rank;
    char suit;
    int value;
} CARD;

typedef struct {
    CARD deck[NUM_CARDS];
    int top;
    int size;
} DECK;

typedef struct {
    int money;
    int status; // call, raise, fold, etc.
    CARD hand[11];
    int numCards;
    int currentBet;
    int value[2]; // 2 values in case player has an ace.
} PLAYER;

typedef struct {
    CARD hand[11];
    int numCards;
    int value[2]; // 2 values in case dealer has an ace.
    int hiddenValue;
} DEALER;


DECK *createDeck();
void insertCards(DECK *);
DECK *shuffleDeck();
void freeDeck(DECK *);
void printDeck(DECK *);

#endif
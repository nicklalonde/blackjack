#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define NUM_CARDS 52
#define NUM_PLAYERS 4


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

int calculateHandValue(CARD hand[], int numCards);
DECK *createDeck();
void insertCards(DECK *deck);
void shuffle(DECK *deck);
char * printCard(CARD card);
void showDealerHand(DEALER *dealer, int hide);
int isBlackJack(PLAYER *player);
void showPlayerHand(PLAYER *me);
void showPlayerStats(PLAYER* me);
void showDealerStats(DEALER *dealer, int m);
PLAYER *createPlayer();
DEALER *createDealer();
void dealCards(PLAYER *player, DEALER* dealer, DECK *deck);
void takePlayerBet(PLAYER *player);
void stand(PLAYER *player);
int hit(CARD cards[], DECK *deck, int numCards);
void dealerTurn(DEALER *dealer, PLAYER *player, DECK *deck);
void determineWinner(DEALER *dealer, PLAYER *player);
void playerTurn(PLAYER *player, DECK *deck, DEALER *dealer, int start);
void reset(PLAYER *player, DECK *deck, DEALER * dealer);

#endif
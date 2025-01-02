#include "blackjack.h"

int calculateHandValue(CARD hand[], int numCards) {
    int total = 0, aces = 0;

    for (int i = 0; i < numCards; i++) {
        
        total += hand[i].value;
        if (hand[i].rank == 14) { // Ace
            aces++;
        }
    }

    // Adjust Aces from 11 to 1 if total exceeds 21
    while (total > 21 && aces > 0) {
        total -= 10;
        aces--;
    }

    return total;
}
DECK *createDeck() {
    DECK *deck = (DECK *) malloc(sizeof(DECK));
    if (!deck) {
        perror("failed to allocate deck memory");
        exit(1);
    }
    deck->size = NUM_CARDS;
    deck->top = 0;

    return deck;
}
void insertCards(DECK *deck) {
    char suits[] = {'H', 'D', 'S', 'C'};
    int index = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 2; j <= 14; j++) {
            if (j == 14){
                CARD card = {j, suits[i], 11};
                deck->deck[index++] = card;
            }
            else if(j > 9){
                CARD card = {j, suits[i], 10};
                deck->deck[index++] = card;
            }
            else {
                CARD card = {j, suits[i], j};
                deck->deck[index++] = card;
            }
        }
    }
}
void shuffle(DECK *deck) {
    srand(time(NULL));
    for (int i = 0; i < deck->size; i++) {
        int swap_index = rand() % deck->size;
        CARD temp = deck->deck[i];
        deck->deck[i] = deck->deck[swap_index];
        deck->deck[swap_index] = temp;
    }
}
DECK *shuffleDeck() {
    DECK *deck = createDeck();
    insertCards(deck);
    shuffle(deck);
    return deck;
}

void printDeck(DECK *deck) {
    for (int i = deck->top; i < deck->size; i++) {
        printf("%d of %c\n", deck->deck[i].rank, deck->deck[i].suit);
    }
}


char * printCard(CARD card) {

    char *string = malloc(sizeof(string) * 20);
    char *suit, rank[3];

    switch(card.suit) {
        case 'H':
            suit = "♥";
            break;
        case 'D':
            suit = "♦";
            break;
        case 'S':
            suit = "♠";
            break;
        case 'C':
            suit = "♣";
            break;
    }

    if(card.rank >= 2 && card.rank <=10) {
        snprintf(string, 20, "%d of %s", card.rank, suit);
        return string;
    }
    else {
        switch(card.rank) {
            case 11:
                strcpy(rank, "J");
                break;
            case 12: 
                strcpy(rank, "Q");
                break;
            case 13:
                strcpy(rank, "K");
                break;
            case 14:
                strcpy(rank, "A");
                break;
        }
    }

    snprintf(string, 20, "%s of %s", rank, suit);
    return string;
}

void showDealerHand(DEALER *dealer, int hide) {
    
    printf("cards\n");
    int value;
    if(hide == 1) { // shows only one card at start.
        char *string = printCard(dealer->hand[0]);
        printf("\t\t%d:   %s\n", 1, string);
        printf("\t\t%d:   hidden\n", 2);
        free(string);
        value = dealer->hand[0].value;
        dealer->hiddenValue = value;

    }
    else {

        for(int i = 0 ; i < dealer->numCards ; i++) {
            char *string = printCard(dealer->hand[i]);
            printf("\t\t%d:   %s\n", i + 1, string);
            free(string);
        }
        value = calculateHandValue(dealer->hand, dealer->numCards);
        
    }
    dealer->value[0] = calculateHandValue(dealer->hand, dealer->numCards);
    printf("\nvalue\n");
    printf("\t             %d", value);
    printf("\n\n");
    
}
int isBlackJack(PLAYER *player) {
    if(player->value[0] == 21) {
        printf("\n\n\t\tblackjack!\n\n");
        return 1;
    }
    return 0;
}
void showPlayerHand(PLAYER *me) {
    char *suit, *rank;
    int ace = 0;
    int value = 0;
    printf("cards\n");
    for(int i = 0; i < me->numCards; i++){
        char *string = printCard(me->hand[i]);
        printf("\t\t%d:   %s\n", i+1, string);
        free(string);
    }
    me->value[0] = calculateHandValue(me->hand, me->numCards);
    printf("\nvalue\n");
    printf("\t             %d", me->value[0]);
    printf("\n");
}
void showPlayerStats(PLAYER* me){
    printf("--------------------     YOU    --------------------\n");
    showPlayerHand(me);
    printf("\nmoney\n");
    printf("                     $%d     \n", me->money);
    printf("\n");
}
void showDealerStats(DEALER *dealer, int m) {
    printf("--------------------   DEALER   --------------------\n");
    showDealerHand(dealer, m);

}

PLAYER *createPlayer() {
    PLAYER *player = malloc(sizeof(PLAYER));
    if(!player) {
        perror("Failed to allocate memory for player.\n");
        exit(1);
    }

    player->money = 500; //start with $500.
    player->numCards = 0;
    player->currentBet = 0;
    player->status = 0; 

    return player;
}
DEALER *createDealer() {
    DEALER *dealer = malloc(sizeof(DEALER));
    if(!dealer) {
        perror("Failed to allocate memory for dealer.\n");
        exit(1);
    }

    dealer->numCards = 0;
    dealer->hiddenValue = 0;
    return dealer;
}

void dealCards(PLAYER *player, DEALER* dealer, DECK *deck) {
    // Player receives card first, then dealer (face up) then
    for(int i = 0; i < 2; i++){   
        player->hand[i] = deck->deck[deck->top++];
        player->numCards++;

        dealer->hand[i] = deck->deck[deck->top++];
        dealer->numCards++;
    }
}

void takePlayerBet(PLAYER *player) {

    printf("Please place your bet ($50 min).\n");
    printf("Your money:     $%d\n", player->money);

    if(player->money < 50) {
        printf("You do not have enough money to play.\n");
        return; 
    }

    int bet, valid = 0;
    do {
        printf("Bet:     ");
        scanf("%d", &bet);
        printf("\n");

        if(bet < 50 || bet > player->money) {
            printf("Invalid bet.\n");
            continue;
        }
        else
            valid = 1;
    } while(valid == 0);

    player->currentBet = bet;
    player->money -= bet;

    printf("You placed a bet for $%d. You now have $%d.\n", bet, player->money);
    return;
}

void stand(PLAYER *player) {
    printf("Stand!\n");
}
int hit(CARD cards[], DECK *deck, int numCards) {
    printf("\n\n\t\tHit!\n\n");

    int total;

    // Add a new card to the player's hand
    cards[numCards++] = deck->deck[deck->top++];

    total = calculateHandValue(cards, numCards);

    printf("\n\n%d", total);

    if (total == 21) {
        printf("\n\n\t\t21!\n\n");
        return total;
    }

    // Show the updated hand value
    printf("\n\n\t\tNew total: %d\n", total);

    // Check if  busts
    if (total > 21) {
        printf("\n\n\t\tBusted with a total of %d!\n", total);
        return -1; //  busts
    }
    return total; // still in the game
}



void dealerTurn(DEALER *dealer, PLAYER *player, DECK *deck) {
    printf("\n\n\t\tDealer's turn begins.\n\n");

    if(dealer->value[0] == 21) { // dealer has natural blackjack
        printf("\n\n\t\tdealer shows blackjack!\n\n");
    }
    else if (dealer->value[0] > player->value[0]) {
        printf("\n\n\t\tdealer stands -- and wins.\n\n");
    }
    else if(dealer->value[0] <= 16) { // the dealer must hit if their value is <= 16
        while(player->value[0] > dealer->value[0]) { // dealer must hit until they beat player or bust
            int total = hit(dealer->hand, deck, dealer->numCards);
            dealer->numCards++;
            dealer->value[0] = total;
            if(total == -1) {
                return; // dealer busted.
            }
        }
    }
    else if (dealer->value[0] > 16) { // they stand;
        printf("\n\n\t\tdealer stands at %d", dealer->value[0]);
    } 

    return;
}



void determineWinner(DEALER *dealer, PLAYER *player) {
    // Dealer busts
    int playerValue = player->value[0];
    int dealerValue = dealer->value[0];

    // Players wins if the dealer busts or the player's cards are valued higher than the dealer's.

    if (dealerValue > 21) { 
        printf("Player wins!\n");
        player->money = player->money + player->currentBet * 2;
    } 
    else if (playerValue <= 21 && playerValue > dealerValue) { 
        printf("Player wins!\n");
        player->money = player->money + player->currentBet * 2;
    } // hands are the same value.
    else if (playerValue == dealerValue) { 
        printf("Push!\n");
        player->money += player->currentBet;
    }
    else { // else the dealer wins.
        printf("Dealer wins!\n");
    }

    return;
}

void playerTurn(PLAYER *player, DECK *deck, DEALER *dealer, int start) {
    int done = 0;

    while(!done) { // player goes until they stand.

        int decision, valid = 0;
        int bj = isBlackJack(player);
        if(!bj){ 
            if(start)
                printf("\n\nYou are at %d. The dealer is at %d.\nWould you like to hit (1) or stand (2) ?\n", player->value[0], dealer->hiddenValue);
            else
                printf("\n\nYou are at %d. The dealer is at %d.\nWould you like to hit (1) or stand (2) ?\n", player->value[0], dealer->value[0]);
            do { // FIX LOOP HERE -- infinite on bad input.
                printf("decision:     ");
                scanf("%d", &decision);
                printf("\n");
                if(decision != 1 && decision != 2) {
                    printf("Invalid decision. Please enter (1) hit or (2) stand.\n");
                    continue;
                }
                else 
                    valid = 1;

            } while(valid == 0);
        }
        else {
            decision = 2;
        }

        if(decision ==  1 && player->value[0] < 21) {
            int total = hit(player->hand, deck, player->numCards);
            player->numCards++;
            player->value[0] = total;
            showPlayerStats(player);
            if(total == -1) {
                return;
            }
            
        }
        else if(decision == 2) {
            stand(player);
            done = 1;
        }
    }

}

void reset(PLAYER *player, DECK *deck, DEALER * dealer) {
        // Clear player hands
        for(int i = 0; i < player->numCards; i ++) {
            player->hand[i].rank = 0;
            player->hand[i].suit = ' ';
            player->hand[i].value = 0;
        }
        player->numCards = 0; player->value[0] = 0;
        
        // Clear dealer
        for(int i  = 0; i < dealer->numCards; i++) {
            dealer->hand[i].rank = 0;
            dealer->hand[i].suit = ' ';
            dealer->hand[i].value = 0;
        }
        dealer->numCards = 0; dealer->value[0] = 0;

        // shuffle deck.

}

int main(int argc, char *argv[]) {

    printf("\n\n-------------------- Blackjack --------------------\n\n");
    printf("\t\tComputer always deals.\n\n");
    printf("\t\tMinimum table bet is $50.\n\n");
    printf("\t\tYou start with $500.\n\n");

    
    int start = 1,  hidden = 1;
    DECK *deck = shuffleDeck();
    PLAYER *player = createPlayer();
    DEALER *dealer = createDealer();

    while(player->money != 0) {
        if(!start) {
            reset(player, deck, dealer);
        }
        takePlayerBet(player);
        dealCards(player, dealer, deck);
        showPlayerStats(player);
        showDealerStats(dealer, 1);
        
        playerTurn(player, deck, dealer, hidden);
        hidden = 0;
        // Dealer's turn
        dealerTurn(dealer, player, deck);

    winner: 
        determineWinner(dealer, player);

        showDealerStats(dealer, 0);
        showPlayerStats(player);
        start = 0;
    }

    free(deck);
    free(player);
    free(dealer);
    return 0;
}

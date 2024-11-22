#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define CARDS_IN_DECK 55
#define JOKER_AMOUNT 3

typedef enum {
    SUIT_CLUBS,
    SUIT_DIAMONDS,
    SUIT_HEARTS,
    SUIT_SPADES,
} suit;

typedef enum {
    RANK_TWO,
    RANK_THREE,
    RANK_FOUR,
    RANK_FIVE,
    RANK_SIX,
    RANK_SEVEN,
    RANK_EIGHT,
    RANK_NINE,
    RANK_TEN,
    RANK_JACK,
    RANK_QUEEN,
    RANK_KING,
    RANK_ACE,
} rank;

struct playing_card {
    suit suit;
    rank rank;
    bool is_joker;
};

// prototypes ----------------------------------------------------
void init_deck(struct playing_card deck[CARDS_IN_DECK]);
void shuffle_deck(struct playing_card deck[CARDS_IN_DECK]);
void order_deck(struct playing_card deck[CARDS_IN_DECK]);

void print_deck(struct playing_card deck[CARDS_IN_DECK]);
void print_card_into(struct playing_card card);

int compare_card_value(const void* a, const void* b);
int calculate_card_value(struct playing_card card);


int main() {
    struct playing_card deck[CARDS_IN_DECK] = {0, 0, false};

    // initializes the deck with all the required cards
    init_deck(deck);
    print_deck(deck);

    // shuffles the deck
    shuffle_deck(deck);
    print_deck(deck);

    order_deck(deck);
    print_deck(deck);

    return EXIT_SUCCESS;
}


void init_deck(struct playing_card deck[CARDS_IN_DECK]) {
    int card_counter = 0;

    // adding standard cards (from suit club to spades, and from rank two to king)
    for (suit suit = SUIT_CLUBS; suit <= SUIT_SPADES; suit++) {
        for (rank rank = RANK_TWO; rank <= RANK_ACE; rank++) {
            deck[card_counter].suit = suit;
            deck[card_counter].rank = rank;
            card_counter++;
        }
    }

    // adding jokers
    for (int i = 0; i < JOKER_AMOUNT; i++) {
        deck[card_counter].is_joker = true;
        card_counter++;
    }
}


// shuffles the deck
void shuffle_deck(struct playing_card deck[CARDS_IN_DECK]) {
    srand(time(NULL));

    // for each card in the deck, swap its position with another random card
    for (int i = 0; i < CARDS_IN_DECK; i++) {
        int j = rand() % CARDS_IN_DECK + 1;

        struct playing_card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}


// orders the deck based on the requirements of the assignment
void order_deck(struct playing_card deck[CARDS_IN_DECK]) {
    qsort(deck, CARDS_IN_DECK, sizeof(deck[0]), compare_card_value);
}


// prints all cards of the deck
void print_deck(struct playing_card deck[CARDS_IN_DECK]) {
    printf("\n~~~ Deck ~~~\n\n");
    for (int i = 0; i < CARDS_IN_DECK; i++) {
        printf("%d: ", i + 1);
        print_card_into(deck[i]);
        printf("\n");
    }

    printf("\n~~~ End of deck ~~~\n\n");
}


// prints a single card
void print_card_into(struct playing_card card) {

    // arrays for suit and rank names, ordered the same as their respective enums
    const char *suit_names[] = {"Clubs", "Diamonds", "Hearts", "Spades"};
    const char *rank_names[] = {"Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten","Jack", "Queen", "King", "Ace"};

    // if the card is a joker nothing should be printed besides this, so the function returns after that
    if (card.is_joker) {
        printf("Joker");
        printf("  [%d]", calculate_card_value(card));
        return;
    }

    // finds the suit of the card and prints the correct name based on that
    for (suit suit = SUIT_CLUBS; suit <= SUIT_SPADES; suit++) {
        if (card.suit == suit) {
            printf("%s, ", suit_names[suit]);
            break;
        }
    }

    // finds the rank of the card and prints the correct name based on that
    for (rank rank = RANK_TWO; rank <= RANK_ACE; rank++) {
        if (card.rank == rank) {
            printf("%s", rank_names[rank]);
            break;
        }
    }

    printf("  [%d]", calculate_card_value(card));
}


// used by qsort to sort cards based on their numerical value
int compare_card_value(const void* a, const void* b) {
    struct playing_card *card_a = (struct playing_card *) a;
    struct playing_card *card_b = (struct playing_card *) b;

    int card_a_value = calculate_card_value(*card_a);
    int card_b_value = calculate_card_value(*card_b);

    return card_a_value - card_b_value;
}


// calculates the numerical value of a card based on what its order in the deck should be.
// this is based on the order of the enums, as they are already ordered in the way we want the deck to be ordered
int calculate_card_value(struct playing_card card) {
    if (card.is_joker) {
        // the joker should be at the end, so the value if the amount of cards in the deck. the order of the joker doesnt matter so they can all have the same value
        return CARDS_IN_DECK;
    } else {
        // the value is the suit number multiplied with the total number of ranks, with the rank number added on top
        return (card.suit * (RANK_ACE + 1) + (card.rank + 1));
    }
}
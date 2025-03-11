#include <stdio.h>
#include <stdlib.h>  // rand
#include <string.h>  // memcpy
#include <stdint.h>  // uint8_t

#define DECK 52
#define HALF (DECK >> 1)

typedef struct stack {
    int count;
    int8_t card[DECK];  // always room for all cards
} Stack;

static const char *face = "_JQKA?";

static Stack deck = {
    .count = DECK,
    .card = {
    1,1,1,1,  // 4 Jacks  = value 1
    2,2,2,2,  // 4 Queens = value 2
    3,3,3,3,  // 4 Kings  = value 3
    4,4,4,4,  // 4 Aces   = value 4
    // 36 numbered cards  = value 0
    }
};

static Stack player[2];
static Stack pile;

static int pick(void)
{
    int x = DECK;
    while (x == DECK)
        x = rand() / ((RAND_MAX + 1u) / DECK);
    return x;
}

static void shuffle(void)
{
    for (int i = 0; i < DECK; ++i) {
        const int j = pick();
        const int tmp = deck.card[i];
        deck.card[i] = deck.card[j];
        deck.card[j] = tmp;
    }
}

static void show(const Stack *const hand)
{
    putchar('[');
    if (hand->count > 0) {
        putchar(face[hand->card[0]]);
        for (int i = 1; i < hand->count; ++i) {
            putchar(',');
            putchar(face[hand->card[i]]);
        }
    }
    putchar(']');
    putchar('\n');
}

static void init(void)
{
    shuffle();
    player[0].count = HALF;
    player[1].count = HALF;
    memcpy(player[0].card, deck.card       , HALF * sizeof *deck.card);
    memcpy(player[1].card, &deck.card[HALF], HALF * sizeof *deck.card);
}

static int play(const int playerindex)
{
    Stack *const p = &player[playerindex];  // helper pointer
    if (p->count <= 0)
        return -1;  // can't play from empty stack
    return pile.card[pile.count++] = p->card[--p->count];
}

static int penalty(const int playerindex, int amount)
{
    Stack *const p = &player[playerindex];  // helper pointer
    while (p->count > 0 && amount-- > 0) {
        const int cardplayed = (pile.card[pile.count++] = p->card[--p->count]);
        if (cardplayed > 0)
            return cardplayed;
    }
    return amount == 0 ? 0 : -1;
}

int main(void)
{
    init();
    show(&player[0]);
    show(&player[1]);
    int turn = 0;
    return 0;
}

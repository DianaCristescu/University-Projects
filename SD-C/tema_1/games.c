#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX 100



typedef struct card {
    int value;
    char number;
    char color;
    struct card* next;
} Card, *Deck;



typedef struct RollingStoneDeck {
    Deck spades;
    Deck hearts;
    Deck clubs;
    Deck diamonds;
} SortedDeck, *RSdeck;



typedef struct PlayerOrder {        //i = (i+1)%4   => rezultate posibile : 0, 1, 2, 3, 0, 1, ...
    RSdeck now;
    struct PlayerOrder *next;
} PlayerOrder, *PplayerOrder;



void printDeck(FILE *out, Deck deck) {

    Deck card = deck;
    while(card != NULL) {
        for(int i=0; i<4; i++) {
            if(card == NULL) break;
            fprintf(out, "%d:%c%c", card->value, card->number, card->color);
            if(i!=3 && card->next != NULL) fprintf(out, " -> ");
            card = card->next;
        }
        fprintf(out, "\n");
    }

}



void printRSdeck(FILE *out, RSdeck deck) {
    fprintf(out, "SPADES:\n");
    printDeck(out, deck->spades);
    fprintf(out, "HEARTS:\n");
    printDeck(out, deck->hearts);
    fprintf(out, "CLUBS:\n");
    printDeck(out, deck->clubs);
    fprintf(out, "DIAMONDS:\n");
    printDeck(out, deck->diamonds);
}



void printAllRSdecks(FILE *out, RSdeck p1, RSdeck p2, RSdeck p3, RSdeck p4) {
    fprintf(out, "\n\nPLAYER1:\n");
    printRSdeck(out, p1);
    fprintf(out, "\n\nPLAYER2:\n");
    printRSdeck(out, p2);
    fprintf(out, "\n\nPLAYER3:\n");
    printRSdeck(out, p3);
    fprintf(out, "\n\nPLAYER4:\n");
    printRSdeck(out, p4);
    fprintf(out, "\n\n");
}



Deck createDeck(char number, char color, int value) {

    Deck deck = NULL;
    deck = (Deck)malloc(sizeof(Card));
    deck->number = number;
    deck->color = color;
    deck->value = value;
    deck->next = NULL;
    return deck;

}



void createCard(Deck* card, char number, char color, int value) {

    *card = createDeck(number, color, value);

}



Deck createPlayingDeck() {

    char* numbers = "KQJT98765432";
    int numbers_len = strlen(numbers);
    char* colors = "shcd";
    int colors_len = strlen(colors);

    Deck playing_deck = createDeck('A', 's', 15);
    Deck card = playing_deck;
    createCard(&card->next, 'A', 'h', 15);
    card = card->next;
    createCard(&card->next, 'A', 'c', 15);
    card = card->next;
    createCard(&card->next, 'A', 'd', 15);
    card = card->next;

    int value = 14;
    for(int i=0; i<numbers_len; i++) {
        for(int j=0; j<colors_len; j++) {
            createCard(&card->next, numbers[i], colors[j], value);
            card = card->next;
        }
        value--;
        if (value == 11) value--;
    }

    return playing_deck;

}



void freeDeck(Deck deck) {

    Deck card;
    while(deck != NULL) {
        card = deck;
        deck = deck->next;                                                                     
        free(card);
    }

}



int deckSize(Deck deck) {

    Deck card = deck;
    int i = 0;
    while(card != NULL) {
        i++;
        card = card->next;
    }
    return i;

}



int getCardPoz(Deck deck, Deck card) {

    int n = deckSize(deck);
    Deck search = deck;
    for(int i=1; i<=n; i++) {
        if(search == card) return i;
        search = search->next;
    }
    return 0;

}



void switchCards(Deck *deck, int poz_c_left, int poz_c_right) {

    int n = deckSize(*deck);
    if(n<=1) return;
    if(poz_c_left > poz_c_right || poz_c_left < 1 || poz_c_right > n) return;

    Deck lcard_prev = *deck;
    for(int i=1; i<poz_c_left-1; i++) lcard_prev = lcard_prev->next;
    Deck rcard_prev = *deck;
    for(int i=1; i<poz_c_right-1; i++) rcard_prev = rcard_prev->next;
    Deck tmp = NULL;

    if(n == 2) {    // schimb singurele 2 elemente
        *deck = rcard_prev;
        rcard_prev = rcard_prev->next;
        rcard_prev->next = lcard_prev;
        lcard_prev->next = NULL;
    } else if(poz_c_left == 1 && poz_c_right == n) {   // schimb capetele
        *deck = rcard_prev->next;
        rcard_prev->next->next = lcard_prev->next;
        tmp = lcard_prev;
        lcard_prev->next = NULL;
        rcard_prev->next = tmp;
    } else if(poz_c_left == 1) {    // schimb inceputul cu un nod oarecare
        *deck = rcard_prev->next;
        tmp = lcard_prev->next;
        lcard_prev->next = rcard_prev->next->next;
        rcard_prev->next->next = tmp;
        rcard_prev->next = lcard_prev;
    } else if(poz_c_right == n) {   // schimb sfarsitul cu un nod oarecare
        tmp = lcard_prev->next;
        lcard_prev->next = rcard_prev->next;
        lcard_prev->next->next = tmp->next;
        rcard_prev->next = tmp;
        tmp->next = NULL;
    } else {    // schimb doua noduri oarecare
        tmp = lcard_prev->next;
        lcard_prev->next = rcard_prev->next;
        rcard_prev->next = tmp;
        tmp = lcard_prev->next->next;
        lcard_prev->next->next = rcard_prev->next->next;
        rcard_prev->next->next = tmp;
    }

}



void mirrorShuffle(Deck *deck) {

    int n = deckSize(*deck);
    if(n<=1) return;

    for(int i=0; i<=n/2; i++) {
        switchCards(deck, i, n-i+1);
    }

}



Deck getDeckEnd(Deck deck, Deck *deck_start_array, int deck_start_array_size) {

    while(deck->next != NULL) {
        for(int i=0; i<deck_start_array_size; i++)
            if(deck->next == deck_start_array[i]) return deck;
        deck = deck->next;
    }

    return deck;

}



Deck nPilesShuffle(Deck deck, int n, int *order) {

    int deck_len = deckSize(deck);
    if(deck_len <=  1) return deck;

    Deck deck_start_array[n];
    Deck card = deck;
    int j = 0;
    for(int i=0; i<deck_len; i++) {
        if(i%(deck_len/n) == 0) {
            deck_start_array[j] = card;
            j++;
        }
        card = card->next;
    }

    Deck deck_ends_array[n];
    for(int i=0; i<n; i++) {
        deck_ends_array[i] = getDeckEnd(deck_start_array[i], deck_start_array, n);
    }

    Deck new_deck = deck_start_array[order[0]-1];
    for(int i=1; i<n; i++) {
        deck_ends_array[order[i-1]-1]->next = deck_start_array[order[i]-1];
    }
    deck_ends_array[order[n-1]-1]->next = NULL; 

    return new_deck;

}



void firstLastInsert(Deck *head, Deck card, int position) { 
    /*
    * position: 1-cap, 0-coada 
    */

    Deck new_card = NULL;
    createCard(&new_card, card->number, card->color, card->value);

    if(deckSize(*head) == 0)
        *head = new_card;
    else {
        if(position) {  // inserare la cap
            new_card->next = *head;
            *head = new_card;
        } else { // inserare la coada
            Deck tmp = *head;
            while(tmp->next != NULL) tmp = tmp->next;
            tmp->next = new_card;
        }
    }

}



void mongeanShuffle(Deck *deck) {

    Deck card = *deck;
    int n = deckSize(*deck);
    Deck new_deck = createDeck(card->number, card->color, card->value);
    for(int i=1; i<n; i++) {
        card = card->next;
        if(i%2 == 0) {
            firstLastInsert(&new_deck, card, 0);
        } else {
            firstLastInsert(&new_deck, card, 1);
        }
    }

    card = *deck;
    *deck = new_deck;
    freeDeck(card);

}



Deck intercalationShuffle(Deck deck) {

    int deck_len = deckSize(deck);
    if(deck_len <=  1) return deck;

    Deck deck_start_array[2];
    Deck card = deck;
    int j = 0;
    for(int i=0; i<deck_len; i++) {
        if(i%(deck_len/2) == 0) {
            deck_start_array[j] = card;
            j++;
        }
        card = card->next;
    }

    Deck deck1 = deck_start_array[0];
    Deck deck2 = deck_start_array[1];
    Deck new_deck = createDeck(deck1->number, deck1->color, deck1->value);
    deck1 = deck1->next;
    for(int i=1; i<deck_len; i++) {
        if(i%2 == 0) {
            card = deck1->next;
            firstLastInsert(&new_deck, deck1, 0);
            deck1 = card;
        } else {
            card = deck2->next;
            firstLastInsert(&new_deck, deck2, 0);
            deck2 = card;
        }
    }

    freeDeck(deck);
    return new_deck;
}



void printArray(int *array, int n, char *name) {
    printf("\n\n%s:\t", name);
    for(int i=0; i<n; i++) {
        printf("%d, ", array[i]);
    }
    printf("\n");
}



void fprintDeck(FILE *ptr, Deck deck) {
    Deck card = deck;
    while(card != NULL) {
        fprintf(ptr, "%c%c", card->number, card->color);
        if(card->next != NULL) fprintf(ptr, " ");
        card = card->next;
    }

}



void shuffle(FILE *in, FILE *out, Deck *playing_deck, char * line) {

    int n = 0;
    int line_len = 0;

    fgets(line, MAX, in);
    n = atoi(line);
    for(int i=0; i<n; i++) {

        fgets(line, MAX, in);
        line_len = strlen(line);
        while(line[line_len-1] <=32) { 
            line[line_len-1] = '\0';
            line_len -= 1;
        }

        if(!strcmp("inversare", line)) {

            mirrorShuffle(playing_deck);

        } else if(!strcmp("intercalare", line)) {

            *playing_deck = intercalationShuffle(*playing_deck);

        } else if(!strcmp("mongean", line)) {

            mongeanShuffle(playing_deck);  

        } else {    //n-Piles

            int decks_nr = atoi(line+strlen("npiles")+1);
            int offset = 0;
            if(decks_nr >= 10) offset = 1;
            int order[decks_nr];
            for(int j=1; j<=decks_nr; j++) {
                order[j-1] = atoi(2*j+line+strlen("npiles")+1+offset);
                if(order[j-1] >= 10) offset += 1;
            }

            *playing_deck = nPilesShuffle(*playing_deck, decks_nr, order);

        }
    }
}



int min(int x, int y) {
    if(x>=y) return y;
    else return x;
}



int getFrequency(Deck deck, int value) {
    int i = 0;
    while(deck != NULL) {
        if(deck->value == value) i++;
        deck = deck->next;
    }
    return i;
}



Deck firstLastInsertFree(Deck *destination, Deck card, int position) {
    firstLastInsert(destination, card, position);
    Deck next = card->next;
    free(card);
    return next;
}



char *war(Deck deck) {

    int deck_len = deckSize(deck);
    Deck card = deck;
    Deck player1 = NULL, player2 = NULL;
    int j = 0;

    for(int i=0; i<deck_len; i++) {
        if(i%(deck_len/2) == 0) {
            if(j == 0)
                firstLastInsert(&player1, card, 1);
            else
                firstLastInsert(&player2, card, 1);
            j++;
        } else if(j == 1) {
            firstLastInsert(&player1, card, 0);
        } else if(j == 2) {
            firstLastInsert(&player2, card, 0);
        }
        card = card->next;
    }

    int i = 1;
    Deck army1 = NULL, army2 = NULL;
    int nr_flipped_cards = 0;

    while(player1 != NULL && player2 != NULL && i<=100) {

        player1 = firstLastInsertFree(&army1, player1, 1);
        player2 = firstLastInsertFree(&army2, player2, 1);

        if(army1->value != army2->value) {

            if(army1->value > army2->value) {
                army1 = firstLastInsertFree(&player1, army1, 0);
                army2 = firstLastInsertFree(&player1, army2, 0);
            } else {
                army2 = firstLastInsertFree(&player2, army2, 0);
                army1 = firstLastInsertFree(&player2, army1, 0);
            }

        } else {

            nr_flipped_cards = min(min(deckSize(player1), deckSize(player2)), army1->value);
            for(int j=0; j<nr_flipped_cards; j++) {
                player1 = firstLastInsertFree(&army1, player1, 1);
                player2 = firstLastInsertFree(&army2, player2, 1);
            }

            if(army1->value > army2->value) {

                while(army1 != NULL) {
                    army1 = firstLastInsertFree(&player1, army1, 0);
                }
                while(army2 != NULL) {
                    army2 = firstLastInsertFree(&player1, army2, 0);
                }

            } else {

                while(army2 != NULL) {
                    army2 = firstLastInsertFree(&player2, army2, 0);
                }
                while(army1 != NULL) {
                    army1 = firstLastInsertFree(&player2, army1, 0);
                }

            }
        }
        i++;
    }

    char numbers[14] = "AKQJT98765432";
    int values[13] = {15, 14, 13, 12, 10, 9, 8, 7, 6, 5, 4, 3, 2};
    char *winner = malloc(4*sizeof(char));
    if(player1 == NULL) {
        winner[0] = '2';
        winner[1] = '\0';
        freeDeck(player1);
        freeDeck(player2);
        return winner;
    } else if(player2 == NULL) {
        winner[0] = '1';
        winner[1] = '\0';
        freeDeck(player1);
        freeDeck(player2);
        return winner;
    } else {
        for(int i=0; i<13; i++) {
            if(getFrequency(player1, values[i]) > getFrequency(player2, values[i])) {
                winner[0] = '1';
                winner[1] = ' ';
                winner[2] = numbers[i];
                winner[3] = '\0';
                freeDeck(player1);
                freeDeck(player2);
                return winner;
            } else if (getFrequency(player1, values[i]) < getFrequency(player2, values[i])) {
                winner[0] = '2';
                winner[1] = ' ';
                winner[2] = numbers[i];
                winner[3] = '\0';
                freeDeck(player1);
                freeDeck(player2);
                return winner;
            }
        }
    }
}



RSdeck createRSdeck() {
    RSdeck deck = NULL;
    deck = malloc(sizeof(SortedDeck));
    deck->spades = NULL;
    deck->hearts = NULL;
    deck->clubs = NULL;
    deck->diamonds = NULL;
    return deck;
}



void addToRSpile(RSdeck *deck, Deck card) {

    Deck new_card = NULL, tmp;
    createCard(&new_card, card->number, card->color, card->value);

    if (new_card->color == 's') {

        if((*deck)->spades == NULL) (*deck)->spades = new_card;
        else {
            tmp = (*deck)->spades;
            (*deck)->spades = new_card;
            (*deck)->spades->next = tmp;
        }
    
    } else if(new_card->color == 'h') {
    
        if((*deck)->hearts == NULL) (*deck)->hearts = new_card;
        else {
            tmp = (*deck)->hearts;
            (*deck)->hearts = new_card;
            (*deck)->hearts->next = tmp;
        }
    
    } else if(new_card->color == 'c') {
    
        if((*deck)->clubs == NULL) (*deck)->clubs = new_card;
        else {
            tmp = (*deck)->clubs;
            (*deck)->clubs = new_card;
            (*deck)->clubs->next = tmp;
        }
    
    } else {
    
        if((*deck)->diamonds == NULL) (*deck)->diamonds = new_card;
        else {
            tmp = (*deck)->diamonds;
            (*deck)->diamonds = new_card;
            (*deck)->diamonds->next = tmp;
        }
    
    }

}



void freeRSdeck(RSdeck deck) {
    freeDeck(deck->spades);
    freeDeck(deck->hearts);
    freeDeck(deck->clubs);
    freeDeck(deck->diamonds);
    free(deck);
}



int isEmpty(RSdeck deck) {
    if(deckSize(deck->spades) != 0 || deckSize(deck->hearts) != 0 ||
    deckSize(deck->clubs) != 0 || deckSize(deck->diamonds) != 0) return 0;
    return 1;
}



char *rollingStone(FILE *out, Deck deck) {

    RSdeck player1 = createRSdeck();
    RSdeck player2 = createRSdeck();
    RSdeck player3 = createRSdeck();
    RSdeck player4 = createRSdeck();

    int j = 0;
    int deck_len = deckSize(deck);
    Deck card = deck;
    for(int i=0; i<deck_len; i++) {
        if(i%(deck_len/4) == 0) {
            j++;
        }
        if(j == 1) {
            addToRSpile(&player1, card);
        } else if(j == 2) {
            addToRSpile(&player2, card);
        } else if(j == 3) {
            addToRSpile(&player3, card);
        } else if(j == 4) {
            addToRSpile(&player4, card);
        }
        card = card->next;
    }

    PplayerOrder player = NULL, tmp_po = NULL;

    player = malloc(sizeof(PlayerOrder));
    player->now = player1;
    player->next = NULL;

    player->next = malloc(sizeof(PlayerOrder));
    player->next->now = player2;
    player->next->next = NULL;

    player->next->next = malloc(sizeof(PlayerOrder));
    player->next->next->now = player3;
    player->next->next->next = NULL;

    player->next->next->next = malloc(sizeof(PlayerOrder));
    player->next->next->next->now = player4;
    player->next->next->next->next = player;


    int i = 0;
    char order[5] = "shcd";
    Deck table = NULL, tmp = NULL;
    int chosen = 0;

    while(!isEmpty(player1) && !isEmpty(player2) &&
    !isEmpty(player3) && !isEmpty(player4)) {


        // printAllRSdecks(out, player1, player2, player3, player4);    //
        if(order[i] == 's') {
            if(deckSize(player->now->spades) > 0) {
                player->now->spades = firstLastInsertFree(&table, player->now->spades, 1);
                chosen = 1;
            }
        }

        if(order[i] == 'h') {
            if(deckSize(player->now->hearts) > 0) {
                player->now->hearts = firstLastInsertFree(&table, player->now->hearts, 1);
                chosen = 1;
            }
        }

        if(order[i] == 'c') {
            if(deckSize(player->now->clubs) > 0) {
                player->now->clubs = firstLastInsertFree(&table, player->now->clubs, 1);
                chosen = 1;
            }
        }

        if(order[i] == 'd') {
            if(deckSize(player->now->diamonds) > 0) {
                player->now->diamonds = firstLastInsertFree(&table, player->now->diamonds, 1);
                chosen = 1;
            }
        }


        if(chosen) {
            player = player->next;

            if(order[i] == 's'){
                while(player->now->spades != NULL) {
                    // if(player->now->spades->next != NULL) break;
                    player->now->spades = firstLastInsertFree(&table, player->now->spades, 1);
                    player = player->next;  
                }
            }
            if(order[i] == 'h'){
                while(player->now->hearts != NULL) {
                    // if(player->now->hearts->next != NULL) break;
                    player->now->hearts = firstLastInsertFree(&table, player->now->hearts, 1);
                    player = player->next; 
                }
            }
            if(order[i] == 'c'){
                while(player->now->clubs != NULL) {
                    // if(player->now->clubs->next != NULL) break;
                    player->now->clubs = firstLastInsertFree(&table, player->now->clubs, 1);
                    player = player->next;
                }
            }
            if(order[i] == 'd'){
                while(player->now->diamonds != NULL) {
                    // if(player->now->diamonds->next != NULL) break;
                    player->now->diamonds = firstLastInsertFree(&table, player->now->diamonds, 1);
                    player = player->next;
                }
            }

            // fprintf(out, "TABLE: ");        //
            // printDeck(out, table);          //
            // fprintf(out, "\n");             //
            if(!isEmpty(player1) && !isEmpty(player2) &&
            !isEmpty(player3) && !isEmpty(player4)) {
                freeDeck(table);
                break;
            }
            while(table != NULL) {
                addToRSpile(&player->now, table);
                tmp = table;
                table = table->next;
                free(tmp);
            }

            chosen = 0;
        }

        i++;
        if(i == 4) i = 0;
    }

    char *winner = malloc(2*sizeof(char));
    if(isEmpty(player1)) {
        winner[0] = '1';
    } else if(isEmpty(player2)) {
        winner[0] = '2';
    } else if(isEmpty(player3)) {
        winner[0] = '3';
    } else {
        winner[0] = '4';
    }
    winner[1] = '\0';

    tmp_po = player;
    player = player->next;
    free(tmp_po);
    tmp_po = player;
    player = player->next;
    free(tmp_po);
    tmp_po = player;
    player = player->next;
    free(tmp_po);
    tmp_po = player;
    free(tmp_po);

    freeRSdeck(player1);
    freeRSdeck(player2);
    freeRSdeck(player3);
    freeRSdeck(player4);
    return winner;

}



char *scarabei(Deck deck) {
    return "scarabei";
}



int main() {

    FILE *in = fopen("in", "r");
    FILE *out = fopen("out", "w");
    if(in == NULL || out == NULL) return 0;
    Deck playing_deck = createPlayingDeck();
    char *line = malloc(MAX*sizeof(char));

    fgets(line, MAX, in);
    int line_len = strlen(line);
    while(line[line_len-1] <=32) { 
        line[line_len-1] = '\0';
        line_len -= 1;
    }

    if(!strcmp("amestecare", line)) {

        shuffle(in, out, &playing_deck, line);
        fprintDeck(out, playing_deck);

    } else if(!strcmp("razboi", line)) {

        shuffle(in, out, &playing_deck, line);
        char *free_str = war(playing_deck);
        fprintf(out, "%s", free_str);
        free(free_str);

    } else if(!strcmp("rollingstone", line)) {

        shuffle(in, out, &playing_deck, line);

        // fprintf(out, "\n\n---ROLLING STONE---\n\nSTART:\n");    //
        // printDeck(out, playing_deck);                           //
        // fprintf(out, "\n\n");                                   //

        char *free_str = rollingStone(out, playing_deck);
        fprintf(out, "%s", free_str);
        free(free_str);

    } else if(!strcmp("scarabei", line)) {

        shuffle(in, out, &playing_deck, line);
        // char *free_str = scarabei(playing_deck);
        fprintf(out, "%s", scarabei(playing_deck));
        // free(free_str);

    }

    freeDeck(playing_deck);
    fclose(in);
    fclose(out);
    free(line);
    return 0;

}



    //#include <stdio.h>
    //#include <math.h>
    // float v[10] = {16.00, 15.87, 15.98, 16.15, 16.04, 16.12, 16.23, 15.95, 16.12, 16.03};
    // float sum = 0;
    // for(int i=0; i<10; i++) {
    //     sum += (v[i]-16.05)*(v[i]-16.05);
    // }
    // printf("\n\n\nSUMA:  %f\n\n\n", sqrt(0.1025001/10));


    
    // Deck playing_deck1 = createPlayingDeck();
    // printf("\n\nNORMAL: (%d)\n", deckSize(playing_deck1));
    // printDeck(playing_deck1);

    // mirrorShuffle(&playing_deck1);
    // printf("MIRROR: (%d)\n", deckSize(playing_deck1));
    // printDeck(playing_deck1);
    // freeDeck(playing_deck1);

    // Deck playing_deck2 = createPlayingDeck();
    // int order[4] = {3, 1, 2, 4};
    // playing_deck2 = nPilesShuffle(playing_deck2, 4, order);
    // printf("N-CARDS: (%d)\n", deckSize(playing_deck2));
    // printDeck(playing_deck2);
    // freeDeck(playing_deck2);

    // Deck playing_deck3 = createPlayingDeck();
    // mongeanShuffle(&playing_deck3);
    // printf("MONGEAN: (%d)\n", deckSize(playing_deck3));
    // printDeck(playing_deck3);
    // freeDeck(playing_deck3);

    // Deck playing_deck4 = createPlayingDeck();
    // playing_deck4 = intercalationShuffle(playing_deck4);
    // printf("INTERCALATION: (%d)\n", deckSize(playing_deck4));
    // printDeck(playing_deck4);
    // freeDeck(playing_deck4);

    // return 0;
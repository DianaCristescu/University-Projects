#include <stdio.h>
#include <string.h>
#include <math.h>
#include "util/scrabble.h"
#include "util/print_board.h"
#define MAX 50
char point_list[] = {1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10};
char game_board[BOARD_SIZE][BOARD_SIZE] = {
{'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
{'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
{'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
{'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
{'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
{'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
{'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
{'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
{'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
{'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
{'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
{'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
{'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
{'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
{'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'}};

char get_input(char *type){
    char inp;
    do{
        inp = getchar();
    } while (!strchr(type, inp));
    return inp;
}

int char_to_int(char c){
    return (int)c - 48;
}

char get_number_input(){
    char numbers[] = "0123456789";
    char input = get_input(numbers);
    int x = char_to_int(input);
    input = getchar();
    if (strchr(numbers, input)){
        x = x*10 + char_to_int(input);
    }
    return x;
}

int points(char c){
    return point_list[(int)c-65];
}

void put_word(int d, int x, int y, char* word_pts){
    char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char input;
    int pts_1 = 0;
    int pts_2 = 0;
    int xx_bonus = 0;
    int yy_bonus = 0;
    strcpy(word_pts, "");
    input = get_input(letters);
    int j = 0;
    while(strchr(letters, input)){
        strncat(word_pts, &input, 1);
        pts_1 = pts_1 + points(input);
        if (pts_1 >= 10){
            pts_2 += 1;
            pts_1 %= 10;
        }
        if (!d){
            if (bonus_board[y][x+j] == 1){
                xx_bonus += 1;
            } else if (bonus_board[y][x+j] == 2){
                yy_bonus += 1;
            }
            game_board[y][x+j] = input;
        } else {
            if (bonus_board[y+j][x] == 1){
                xx_bonus += 1;
            } else if (bonus_board[y+j][x] == 2){
                yy_bonus += 1;
            }
            game_board[y+j][x] = input;
        }
        input = getchar();
        j++;
    }
    char char_pts_1 = pts_1 + '0';
    char char_pts_2 = pts_2 + '0';
    char char_xx_bonus = xx_bonus + '0';
    char char_yy_bonus = yy_bonus + '0';
    strncat(word_pts, &char_pts_2, 1);
    strncat(word_pts, &char_pts_1, 1);
    strncat(word_pts, &char_xx_bonus, 1);
    strncat(word_pts, &char_yy_bonus, 1);
}

int get_pts(char* word_pts){
    char pts_1;
    char pts_2 = 0;
    pts_1 = word_pts[strlen(word_pts)-3];
    pts_2 = word_pts[strlen(word_pts)-4];
    return (char_to_int(pts_1) + char_to_int(pts_2) * 10);
}

int check_XX(char* word_pts, char* xx){
    char word[MAX] = "";
    strncpy(word, word_pts, strlen(word_pts) - 4);
    if (strstr(word, xx)){
        return 1;
    } else {
        return 0;
    }
}

int check_YY(char* word_pts, char* yy){
    char sufix[MAX] = "";
    strncpy(sufix, word_pts + strlen(word_pts) - 6, 2);
    if (!strcmp(sufix, yy)){
        return 1;
    } else{
        return 0;
    }
}

int bonus_XX(char* word_pts){
    char bonus_xx;
    bonus_xx = word_pts[strlen(word_pts) -2];
    return char_to_int(bonus_xx);
}

int bonus_YY(char* word_pts){
    char bonus_yy;
    bonus_yy = word_pts[strlen(word_pts) -1];
    return char_to_int(bonus_yy);
}

void add_played_word(char* word_pts, char* words_played){
    char word[MAX] = "";
    strncpy(word, word_pts, strlen(word_pts) - 4);
    strcpy(words_played, word);
}

int check_tile(int y, int x){
    char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (x>=BOARD_SIZE || y>=BOARD_SIZE){
        return 1;
    } else if (strchr(letters, game_board[y][x])){
        return 1;
    } else{
        return 0;
    }
}

int check_space_horizontally(char* word, int y, int x){
    int len_w = strlen(word);
    int rez = 1;
    x += 1;
    for (int i=x; i<len_w+x-1; i++){
        if (check_tile(y, i)){
            rez = 0;
        }
    }
    return rez;
}

int check_space_vertically(char* word, int y, int x){
    int len_w = strlen(word);
    int rez = 1;
    y += 1;
    for (int i=y; i<len_w+y-1; i++){
        if (check_tile(i, x)){
            rez = 0;
        }
    }
    return rez;
}

void put_played_word(char* word, int y, int x, int direction){
    int len_w = strlen(word);
    for (int i=0; i<len_w; i++){
        if (!direction){
            game_board[y][x + i] = word[i];
        } else{
            game_board[y + i][x] = word[i];
        }
    }
}

int is_not_played(char* word, char words_played[2*MAX + MAX/2][MAX],
 int nr_of_played_words){
    for (int i=0; i<nr_of_played_words; i++){
        if (!strcmp(word, words_played[i])){
            return 0;
        }
    }
    return 1;
}

int play_word(char words_played[2*MAX + MAX/2][MAX], int nr_of_played_words){
    char letter = '0';
    for (int i=0; i<NUM_WORDS; i++){
        letter = words[i][0];
        for (int y=0; y<BOARD_SIZE; y++){
            for (int x=0; x<BOARD_SIZE; x++){
                if (check_tile(y, x)){
                    if (letter == game_board[y][x] && is_not_played(words[i],
                     words_played, nr_of_played_words)){
                        if (check_space_horizontally(words[i], y, x)){
                            put_played_word(words[i], y, x, 0);
                            return 1;
                        } else if (check_space_vertically(words[i], y, x)){
                            put_played_word(words[i], y, x, 1);
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int check_xx(char* word, char* xx){
    if (strstr(word, xx)){
        return 1;
    } else {
        return 0;
    }
}

int check_yy(char* word, char* yy){
    char sufix[MAX] = "";
    strncpy(sufix, word + strlen(word) - 2, 2);
    if (!strcmp(sufix, yy)){
        return 1;
    } else{
        return 0;
    }
}

int calculate_points(char* word, int y, int x, int direction,
 char* xx, char* yy){
    int pts = 0;
    int len_w = strlen(word);
    int bonus_1 = 0;
    int bonus_2 = 0;
    for (int i=0; i<len_w; i++){
        if (!direction){
            pts += points(word[i]);
            if (bonus_board[y][x + i] == 1){
                bonus_1++;
            } else if (bonus_board[y][x + i] == 2){
                bonus_2++;
            }
        } else{
            pts += points(word[i]);
            if (bonus_board[y + i][x] == 1){
                bonus_1++;
            } else if (bonus_board[y + i][x] == 2){
                bonus_2++;
            }
        }
    }
    if (check_xx(word, xx) && check_yy(word, yy)){
        pts = pow(2, bonus_1) * pow(3, bonus_2) * pts;
    } else if (bonus_1 && check_xx(word, xx)){
        pts = pow(2, bonus_1) * pts;
    } else if (bonus_2 && check_yy(word, yy)){
        pts = pow(3, bonus_2) * pts;
    }
    return pts;
}

int play_word_with_points(char words_played[2*MAX + MAX/2][MAX],
 int nr_of_played_words, char* xx, char* yy){
    int best_score = 0;
    char best_word[MAX] = "";
    int best_x = 0;
    int best_y = 0;
    int best_d = 0;
    char letter = '0';
    for (int i=0; i<NUM_WORDS; i++){
        letter = words[i][0];
        for (int y=0; y<BOARD_SIZE; y++){
            for (int x=0; x<BOARD_SIZE; x++){
                if (check_tile(y, x)){
                    if (letter == game_board[y][x] && is_not_played(words[i],
                     words_played, nr_of_played_words)){
                        if (check_space_horizontally(words[i], y, x)){
                            if (calculate_points(words[i], y, x, 0, xx, yy) > best_score){
                                best_score = calculate_points(words[i], y, x, 0, xx, yy);
                                strcpy(best_word, words[i]);
                                best_x = x;
                                best_y = y;
                                best_d = 0;
                            } else if (calculate_points(words[i], y, x, 0, xx, yy) == best_score){
                                if (y < best_y){
                                    best_score = calculate_points(words[i], y, x, 0, xx, yy);
                                    strcpy(best_word, words[i]);
                                    best_x = x;
                                    best_y = y;
                                    best_d = 0;
                                } else if (x < best_x && y == best_y){
                                    best_score = calculate_points(words[i], y, x, 0, xx, yy);
                                    strcpy(best_word, words[i]);
                                    best_x = x;
                                    best_y = y;
                                    best_d = 0;
                                }
                            }
                        }
                        if (check_space_vertically(words[i], y, x)){
                            if (calculate_points(words[i], y, x, 1, xx, yy) > best_score){
                                best_score = calculate_points(words[i], y, x, 1, xx, yy);
                                strcpy(best_word, words[i]);
                                best_x = x;
                                best_y = y;
                                best_d = 1;
                            } else if (calculate_points(words[i], y, x, 1, xx, yy) == best_score){
                                if (y < best_y){
                                    best_score = calculate_points(words[i], y, x, 1, xx, yy);
                                    strcpy(best_word, words[i]);
                                    best_x = x;
                                    best_y = y;
                                    best_d = 1;
                                } else if (x < best_x && y == best_y){
                                    best_score = calculate_points(words[i], y, x, 1, xx, yy);
                                    strcpy(best_word, words[i]);
                                    best_x = x;
                                    best_y = y;
                                    best_d = 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    put_played_word(best_word, best_y, best_x, best_d);
    return best_score;
}

int main(){
    char input = getchar();
    int N, Y, X, D;
    int score_player1 = 0;
    int score_player2 = 0;
    char word_pts[MAX] = "";
    char XX[3] = "";
    char YY[3] = "";
    char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int pts = 0;
    char words_played[2*MAX + MAX/2][MAX];
    int nr_of_words_played = 0;

    if(input == '0'){

        print_board(game_board);

    } else if (input == '1'){
        N = get_number_input();
        for (int i=0; i<N; i++){
            Y = get_number_input();
            X = get_number_input();
            D = get_number_input();
            put_word(D, X, Y, word_pts);
        }
        print_board(game_board);

    } else if (input == '2'){

        N = get_number_input();
        for (int i=0; i<N; i++){
            Y = get_number_input();
            X = get_number_input();
            D = get_number_input();
            put_word(D, X, Y, word_pts);
            if (i%2 == 0){
                score_player1 = score_player1 + get_pts(word_pts);
            } else{
                score_player2 = score_player2 + get_pts(word_pts);
            }
        }
        printf("Player 1: %d Points\n", score_player1);
        printf("Player 2: %d Points\n", score_player2);

    } else if (input == '3'){

        input = get_input(letters);
        strcpy(XX, &input);
        input = get_input(letters);
        strcat(XX, &input);
        input = get_input(letters);
        strcpy(YY, &input);
        input = get_input(letters);
        strcat(YY, &input);
        N = get_number_input();
        for (int i=0; i<N; i++){
            Y = get_number_input();
            X = get_number_input();
            D = get_number_input();
            put_word(D, X, Y, word_pts);
            if (check_XX(word_pts, XX) && check_YY(word_pts, YY)){
                pts = pow(2, bonus_XX(word_pts)) * pow(3, bonus_YY(word_pts))
                 * get_pts(word_pts);
            } else if (bonus_XX(word_pts) && check_XX(word_pts, XX)){
                pts = pow(2, bonus_XX(word_pts)) * get_pts(word_pts);
            } else if (bonus_YY(word_pts) && check_YY(word_pts, YY)){
                pts = pow(3, bonus_YY(word_pts)) * get_pts(word_pts);
            } else{
                pts = get_pts(word_pts);
            }
            if (i%2 == 0){
                score_player1 = score_player1 + pts;
            } else{
                score_player2 = score_player2 + pts;
            }
        } 
        printf("Player 1: %d Points\n", score_player1);
        printf("Player 2: %d Points\n", score_player2);

    } else if (input == '4'){

        input = get_input(letters);
        strcpy(XX, &input);
        input = get_input(letters);
        strcat(XX, &input);
        input = get_input(letters);
        strcpy(YY, &input);
        input = get_input(letters);
        strcat(YY, &input);
        N = get_number_input();
        for (int i=0; i<N; i++){
            Y = get_number_input();
            X = get_number_input();
            D = get_number_input();
            put_word(D, X, Y, word_pts);
            add_played_word(word_pts, words_played[nr_of_words_played]);
            nr_of_words_played += 1;
        } 
        play_word(words_played, nr_of_words_played);
        print_board(game_board);

    } else if(input == '5'){

        input = get_input(letters);
        strcpy(XX, &input);
        input = get_input(letters);
        strcat(XX, &input);
        input = get_input(letters);
        strcpy(YY, &input);
        input = get_input(letters);
        strcat(YY, &input);
        N = get_number_input();
        for (int i=0; i<N; i++){
            Y = get_number_input();
            X = get_number_input();
            D = get_number_input();
            put_word(D, X, Y, word_pts);
            add_played_word(word_pts, words_played[nr_of_words_played]);
            nr_of_words_played += 1;
            if (check_XX(word_pts, XX) && check_YY(word_pts, YY)){
                pts = pow(2, bonus_XX(word_pts)) * pow(3, bonus_YY(word_pts))
                 * get_pts(word_pts);
            } else if (bonus_XX(word_pts) && check_XX(word_pts, XX)){
                pts = pow(2, bonus_XX(word_pts)) * get_pts(word_pts);
            } else if (bonus_YY(word_pts) && check_YY(word_pts, YY)){
                pts = pow(3, bonus_YY(word_pts)) * get_pts(word_pts);
            } else{
                pts = get_pts(word_pts);
            }
            if (i%2 == 0){
                score_player1 = score_player1 + pts;
            } else{
                score_player2 = score_player2 + pts;
            }
        } 
        score_player2 += play_word_with_points(words_played, nr_of_words_played, XX, YY);
        if (score_player2 >= score_player1){
            print_board(game_board);
        } else{
            printf("Fail!\n");
        }

    } else if(input == '6'){

    }
}
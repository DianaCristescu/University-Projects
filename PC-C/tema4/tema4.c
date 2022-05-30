#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void* (* Abilities[4])(void* x) = {RotateMatrix, DecodeString, InvertArray, KillPlayer};

char *fromEnumtoString(PlayerRoles playerRole)
{
	char *str = calloc(MAX_LEN_STR_ATR, sizeof(char));
	switch(playerRole)
	{
		case Rotator:
			str = strcpy(str, "Rotator");
			break;
		case Decoder:
			str = strcpy(str, "Decoder");
			break;
		case Invertor:
			str = strcpy(str, "Invertor");
			break;
		case Impostor:
			str = strcpy(str, "Impostor");
			break;
	}
	return str;
}

// Task 1
void* RotateMatrix(void *input)
{
	char* str = malloc(MAX_LEN_STR_OUT);
	char* i_char = malloc(MAX_LEN_STR_ATR);
	char* j_char = malloc(MAX_LEN_STR_ATR);
	str = strcpy(str, "");
	int* n = (int*)input;
	for (int i=1; i<=*n; i++){
		for (int j=*n; j>=1; j--){
			sprintf(j_char, "%d", j);
			sprintf(i_char, "%d", i);
			str = strcat(str, j_char);
			str = strcat(str, i_char);
			str = strcat(str, " ");
		}
		str = strcat(str, "\n");
	}
	return (void*)str;
}

// Task 2
void *DecodeString(void *input)
{
	char* str = (char*)input;
	char* str_tok;
	int sum = 0;
	char* sum_char = malloc(MAX_LEN_STR_OUT);
	str_tok = strtok(str, "_");
	while (str_tok != NULL){
		sum += atoi(str_tok);
		str_tok = strtok(NULL, "_");
	}
	sprintf(sum_char, "%d", sum);
	return (void*)sum_char;
}

//Task 3
void *InvertArray(void *input)
{
	int* vect = (int*)input;
	char* vect_char = malloc(MAX_LEN_STR_OUT);
	strcpy(vect_char, "");
	char* character = malloc(MAX_LEN_STR_ATR);
	int len = vect[0];
	int c;

	if ((len)%2 == 0){
		for (int k=1; k<=len; k+=2){
			c = vect[k];
			vect[k] = vect[k+1];
			vect[k+1] = c;
		}
	} else{
		for (int k=1; k<=(len)/2; k++){
			c = vect[len - k + 1];
			vect[len - k + 1] = vect[k];
			vect[k] = c;
		}
	}

	for (int i=1; i<=len; i++){
		sprintf(character, "%d", vect[i]);
		strcat(vect_char, character);
		if (i != len){
			strcat(vect_char, " ");
		}
	}
	return (void*)vect_char;
}

//Task 4
Player *allocPlayer()
{
	Player* player = malloc(sizeof(Player));
	player->name = malloc(MAX_LEN_STR_ATR);
	player->color = malloc(MAX_LEN_STR_ATR);
	player->hat = malloc(MAX_LEN_STR_ATR);
	player->alive = 1;
	return (void*)player;
}

//Task 4
Game *allocGame()
{
	Game* game = malloc(sizeof(Game));
	game->name = malloc(MAX_LEN_STR_ATR);
	return (void*)game;
}

//Task 5
Player *ReadPlayer(FILE *inputFile)
{
	Player *player = (Player*)allocPlayer();
	char *nr_locations = malloc(MAX_LEN_STR_ATR);
	char *locations = malloc(MAX_LEN_STR_ATR);
	char *role = malloc(MAX_LEN_STR_ATR);
	char *str_tok;

	fgets(player->name, MAX_LEN_STR_ATR, inputFile);
	player->name[strlen(player->name)-1] = '\0';
	fgets(player->color, MAX_LEN_STR_ATR, inputFile);
	player->color[strlen(player->color)-1] = '\0';
	fgets(player->hat, MAX_LEN_STR_ATR, inputFile);
	player->hat[strlen(player->hat)-1] = '\0';

	fgets(nr_locations, MAX_LEN_STR_ATR, inputFile);
	player->numberOfLocations = atoi(nr_locations);
	
	player->locations = malloc(sizeof(Location)*player->numberOfLocations);
	int i = 0;
	int true = 1;
	fgets(locations, MAX_LEN_STR_ATR, inputFile);
	str_tok = strtok(locations, "(), ");
	while (str_tok != NULL){
		if (true){
			player->locations[i].x = atoi(str_tok);
			true = 0;
		} else{
			player->locations[i].y = atoi(str_tok);
			true = 1;
			i++;
		}
		str_tok = strtok(NULL, "(), ");
	}

	fgets(role, MAX_LEN_STR_ATR, inputFile);
	if (role[1]=='o'){
		player->playerRole = Rotator;
	} else if (role[1]=='e'){
		player->playerRole = Decoder;
	} else if (role[1]=='n'){
		player->playerRole = Invertor;
	} else{
		player->playerRole = Impostor;
	}

	return (void*)player;
}

// Task 5
Game *ReadGame(FILE *inputFile)
{
	Game* game = (Game*)allocGame();
	char *get_line = malloc(MAX_LEN_STR_ATR);
	int numar_line;

	fgets(game->name, MAX_LEN_STR_ATR, inputFile);
	game->name[strlen(game->name)-1] = '\0';

	fgets(get_line, MAX_LEN_STR_ATR, inputFile);
	numar_line = atoi(get_line);
	game->killRange = numar_line;

	fgets(get_line, MAX_LEN_STR_ATR, inputFile);
	numar_line = atoi(get_line);
	game->numberOfCrewmates = numar_line;
	
	game->crewmates = malloc(sizeof(Player)*game->numberOfCrewmates);

	for (int i=0; i<game->numberOfCrewmates; i++){
		game->crewmates[i] = (Player*)ReadPlayer(inputFile);
	}
	game->impostor = (Player*)ReadPlayer(inputFile);

	return (void*)game;
}

// Task 6
void WritePlayer(Player *player, FILE *outputFile)
{
	char *name = malloc(MAX_LEN_STR_ATR);
	char *color = malloc(MAX_LEN_STR_ATR);
	char *hat = malloc(MAX_LEN_STR_ATR);
	char *role = malloc(MAX_LEN_STR_ATR);

	name = player->name;
	color = player->color;
	hat = player->hat;

	if (!strchr("abcdefghijklmnopqrstuvwxyz", name[strlen(name)-1])){
		name[strlen(name)-1] = '\0';
		color[strlen(color)-1] = '\0';
		hat[strlen(hat)-1] = '\0';
	}

	if (player->playerRole == 0){
		role = "Rotator";
	} else if (player->playerRole == 1){
		role = "Decoder";
	} else if (player->playerRole == 2){
		role = "Invertor";
	} else{
		role = "Impostor";
	}

	fprintf(outputFile, "Player %s with color %s, hat %s and role %s has entered the game.\n", name, color, hat, role);
	fprintf(outputFile, "Player's locations:");
	for (int i=0; i<player->numberOfLocations; i++){
		fprintf(outputFile, " ");
		fprintf(outputFile, "(%d,%d)", player->locations[i].x, player->locations[i].y);
	}
	fprintf(outputFile, "\n");
	
	return;
}

// Task 6
void WriteGame(Game *game, FILE *outputFile)
{
	if (!strchr("abcdefghijklmnopqrstuvwxyz", game->name[strlen(game->name)-1])){
		game->name[strlen(game->name)-1] = '\0';
	}
	fprintf(outputFile, "Game %s has just started!\n", game->name);
	fprintf(outputFile, "\tGame options:\n");
	fprintf(outputFile, "Kill Range: %d\n", game->killRange);
	fprintf(outputFile, "Number of crewmates: %d\n\n", game->numberOfCrewmates);
	
	fprintf(outputFile, "\tCrewmates:\n");
	for (int i=0; i<game->numberOfCrewmates; i++){
		WritePlayer(game->crewmates[i], outputFile);
	}

	fprintf(outputFile, "\n");
	fprintf(outputFile, "\tImpostor:\n");
	WritePlayer(game->impostor, outputFile);

	return;
}

//Task 7
void *KillPlayer(void *input)
{
	Game *game = (Game*)input;
	int distance;
	int x_crewmate;
	int y_crewmate;
	int x_impostor;
	int y_impostor;
	char *output = malloc(MAX_LEN_STR_ATR);
	char *impostor_name = malloc(MAX_LEN_STR_ATR);
	char *crewmate_name = malloc(MAX_LEN_STR_ATR);
	int minDist = __INT_MAX__;
	int toBeKilled = -1;
	x_impostor = game->impostor->locations[game->impostor->indexOfLocation].x;
	y_impostor = game->impostor->locations[game->impostor->indexOfLocation].y;
	for (int i=0; i<game->numberOfCrewmates; i++){
		if (game->crewmates[i]->alive){
			x_crewmate = game->crewmates[i]->locations[game->crewmates[i]->indexOfLocation].x;
			y_crewmate = game->crewmates[i]->locations[game->crewmates[i]->indexOfLocation].y;
			if (x_crewmate>=x_impostor){
				distance = x_crewmate - x_impostor;
			} else{
				distance = x_impostor - x_crewmate;
			}
			if (y_crewmate>=y_impostor){
				distance += y_crewmate - y_impostor;
			} else{
				distance += y_impostor - y_crewmate;
			}
			if ((distance<=game->killRange) & (distance <= minDist)){
				toBeKilled = i;
				minDist = distance;
			}
		}
	}

	if(toBeKilled == -1){
		impostor_name = game->impostor->name;
		if (!strchr("abcdefghijklmnopqrstuvwxyz", impostor_name[strlen(impostor_name)-1])){
			impostor_name[strlen(impostor_name)-1] = '\0';
		}
		sprintf(output, "Impostor %s couldn't kill anybody.", impostor_name);
		return (void*)output;
	}else{
		game->crewmates[toBeKilled]->alive = 0;
		crewmate_name = game->crewmates[toBeKilled]->name;
		impostor_name = game->impostor->name;
		if (!strchr("abcdefghijklmnopqrstuvwxyz", crewmate_name[strlen(crewmate_name)-1])){
			crewmate_name[strlen(crewmate_name)-1] = '\0';
			impostor_name[strlen(impostor_name)-1] = '\0';
		}
		sprintf(output, "Impostor %s has just killed crewmate %s from distance %d.", impostor_name, crewmate_name, minDist);
		return (void*)output;
	}
}

// Task 8
void CalcuateNextCycleOfGame(Game *game, FILE *outputFile, void **inputMatrix)
{
	// TODO
	return;
}

// Task 9
void FreePlayer(Player *player)
{
	// TODO
	return;
}

// Task 9
void FreeGame(Game *game)
{
	// TODO
	return;
}
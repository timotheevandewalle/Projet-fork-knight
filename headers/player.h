#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Player {
    char username[30];
    int level;
    int spicyIndex;
    int numGames;
    int numWins;
    int numLosses;
    int inQueue;
} Player;

// Fonctions de création/destruction
Player* createPlayer(const char* username);
void destroyPlayer(Player* player);

// Fonctions d'affichage
void displayPlayer(const Player* player);
void displayPlayerMini(const Player* player);

#endif // PLAYER_H

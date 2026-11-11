#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Player {

    char username[30];      // Pseudo du joueur
    int level;              // Niveau (1-100)
    int spicyIndex;         // Score de compétence
    int numGames;           // Total de parties
    int numWins;            // Victoires
    int numLosses;          // Défaites
    int inQueue;            // En file ? (0=non, 1=oui)
} Player;

// Fonctions de création/destruction
Player* createPlayer(const char* username);
void destroyPlayer(Player* player);

// Fonctions d'affichage
void displayPlayer(const Player* player);
void displayPlayerMini(const Player* player);

#endif // PLAYER_H

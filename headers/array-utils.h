#ifndef ARRAY_UTILS_H
#define ARRAY_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"

#define MAX_PLAYERS 100

/* Part 1: Static array management */
int addPlayer(Player* players[], int* count, Player* player);
int searchPlayerByUsername(Player* players[], int count, const char* username);
int removePlayer(Player* players[], int* count, const char* username);
void sortPlayersBySpicyIndex(Player* players[], int count);
void displayAllPlayers(Player* players[], int count);

/* Helper function (optional but recommended) */
int comparePlayer(const Player* p1, const Player* p2);

/* Part 2: Dynamic array management */
Player** createPlayerArray(int size);
void destroyPlayerArray(Player** array, int size);

/* Part 2: File I/O */
Player** loadPlayers(const char* filename, int* numPlayers);
void savePlayers(const char* filename, Player** array, int size);

#endif // ARRAY_UTILS_H

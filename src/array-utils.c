#include "../headers/array-utils.h"

int addPlayer(Player* players[], int* count, Player* player) {
    (void)players;
    (void)count;
    (void)player;
    return 0;
}

int searchPlayerByUsername(Player* players[], int count, const char* username) {
    (void)players;
    (void)count;
    (void)username;
    return -1;
}

int removePlayer(Player* players[], int* count, const char* username) {
    (void)players;
    (void)count;
    (void)username;
    return 0;
}

int comparePlayer(const Player* p1, const Player* p2) {
    (void)p1;
    (void)p2;
    return 0;
}

void sortPlayersBySpicyIndex(Player* players[], int count) {
    (void)players;
    (void)count;
}

void displayAllPlayers(Player* players[], int count) {
    (void)players;
    (void)count;
}

// ============================================================================
// Part 2: Dynamic array management
// ============================================================================

Player** createPlayerArray(int size) {
    (void)size;
    return NULL;
}

void destroyPlayerArray(Player** array, int size) {
    (void)array;
    (void)size;
}

// ============================================================================
// Part 2: File I/O
// ============================================================================

Player** loadPlayers(const char* filename, int* numPlayers) {
    (void)filename;
    (void)numPlayers;
    return NULL;
}

void savePlayers(const char* filename, Player** array, int size) {
    (void)filename;
    (void)array;
    (void)size;
}

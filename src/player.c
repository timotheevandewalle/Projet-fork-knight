#include "../headers/player.h"


Player* createPlayer(const char* username) {
    (void)username;  // Évite le warning de variable non utilisée
    //Vérifier paramètre
    if (username == NULL) {
        return NULL;
    }

    // Allocation dynamique
    Player* p = malloc(sizeof(Player));
    if (p == NULL) {
        return NULL; // Échec allocation
    }

    // Copier le username de manière sécurisée
    strncpy(p->username, username, sizeof(p->username) - 1);
    p->username[sizeof(p->username) - 1] = '\0'; // garantir terminaison

    // Initialiser les champs
    p->level = 1;
    p->spicyIndex = 1000;
    p->numGames = 0;
    p->numWins = 0;
    p->numLosses = 0;
    p->inQueue = 0;

    return p;

}

void destroyPlayer(Player* player) {
    (void)player;
    if (player == NULL) {
        return; // rien à libérer
    }
    free(player);
}

void displayPlayer(const Player* player) {
    (void)player;
    if (player == NULL) {
        return;
    }
    printf("==========================================\n");
    printf("Player: %s\n", player->username);
    printf("Level: %d | Spicy Index: %d\n", player->level, player->spicyIndex);
    printf("Games: %d | Wins: %d | Losses: %d\n",
           player->numGames, player->numWins, player->numLosses);
    printf("In Queue: %s\n", player->inQueue ? "Yes" : "No");
    printf("==========================================\n");
}

void displayPlayerMini(const Player* player) {
    (void)player;
    if (player == NULL) {
        return;
    }
    printf("[%s | Lvl: %d | SI: %d]\n", player->username, player->level, player->spicyIndex);
}

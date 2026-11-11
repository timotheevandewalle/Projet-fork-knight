#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/array-utils.h"
#include "../headers/player.h"

/* ============================================================================
 * Partie 1 : tableau de pointeurs sur joueurs
 * ============================================================================ */

// Retourne 1 si l'ajout a reussi, 0 sinon
int addPlayer(Player* players[], int* count, Player* player) {
    if (players == NULL || count == NULL || player == NULL) {
        return 0;
    }
    if (*count >= MAX_PLAYERS) {
        return 0;   // tableau plein
    }
    if (searchPlayerByUsername(players, *count, player->username) != -1) {
        return 0;   // pseudo deja utilise
    }

    players[*count] = player;
    (*count)++;
    return 1;
}

// Retourne l'index du joueur, ou -1 s'il n'existe pas
int searchPlayerByUsername(Player* players[], int count, const char* username) {
    if (players == NULL || username == NULL) {
        return -1;
    }

    for (int i = 0; i < count; i++) {
        if (players[i] != NULL && strcmp(players[i]->username, username) == 0) {
            return i;
        }
    }
    return -1;
}

// Retourne 1 si le joueur a ete supprime (et libere), 0 sinon
int removePlayer(Player* players[], int* count, const char* username) {
    if (players == NULL || count == NULL || username == NULL) {
        return 0;
    }

    int index = searchPlayerByUsername(players, *count, username);
    if (index == -1) {
        return 0;
    }

    destroyPlayer(players[index]);

    // Decalage vers la gauche
    for (int i = index; i < *count - 1; i++) {
        players[i] = players[i + 1];
    }
    players[*count - 1] = NULL;   // evite un double free dans destroyPlayerArray
    (*count)--;
    return 1;
}

// Ordre decroissant de spicyIndex (negatif si p1 doit passer avant p2)
int comparePlayer(const Player* p1, const Player* p2) {
    if (p1 == NULL && p2 == NULL) return 0;
    if (p1 == NULL) return 1;    // les NULL a la fin
    if (p2 == NULL) return -1;

    if (p1->spicyIndex > p2->spicyIndex) return -1;
    if (p1->spicyIndex < p2->spicyIndex) return 1;
    return 0;   // egalite : le tri par insertion est stable, l'ordre d'origine est conserve
}

// Tri par insertion (stable)
void sortPlayersBySpicyIndex(Player* players[], int count) {
    if (players == NULL || count <= 1) {
        return;
    }

    for (int i = 1; i < count; i++) {
        Player* key = players[i];
        int j = i - 1;
        while (j >= 0 && comparePlayer(key, players[j]) < 0) {
            players[j + 1] = players[j];
            j--;
        }
        players[j + 1] = key;
    }
}

void displayAllPlayers(Player* players[], int count) {
    printf("\n=== Liste des joueurs ===\n");

    if (players == NULL || count <= 0) {
        printf("Aucun joueur enregistre.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        if (players[i] != NULL) {
            displayPlayerMini(players[i]);
        }
    }
    printf("Total: %d joueur%s\n", count, (count > 1) ? "s" : "");
}

/* ============================================================================
 * Partie 2 : tableau dynamique
 * ============================================================================ */

// Tableau de size pointeurs, tous initialises a NULL
Player** createPlayerArray(int size) {
    if (size <= 0) {
        return NULL;
    }
    return calloc((size_t)size, sizeof(Player*));
}

// Libere chaque joueur puis le tableau
void destroyPlayerArray(Player** array, int size) {
    if (array == NULL) {
        return;
    }
    for (int i = 0; i < size; i++) {
        destroyPlayer(array[i]);   // sans effet si NULL
    }
    free(array);
}

/* ============================================================================
 * Partie 2 : fichiers
 * Format : 1re ligne = nombre de joueurs, puis
 *          "username level spicyIndex numGames numWins numLosses inQueue"
 * ============================================================================ */

Player** loadPlayers(const char* filename, int* numPlayers) {
    if (numPlayers == NULL) {
        return NULL;
    }
    *numPlayers = 0;
    if (filename == NULL) {
        return NULL;
    }

    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier \"%s\".\n", filename);
        return NULL;
    }

    int n = 0;
    if (fscanf(f, "%d", &n) != 1 || n <= 0) {
        fclose(f);
        return NULL;
    }

    Player** array = createPlayerArray(n);
    if (array == NULL) {
        fclose(f);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        char name[30];
        int level, spicy, games, wins, losses, queue;

        if (fscanf(f, "%29s %d %d %d %d %d %d",
                   name, &level, &spicy, &games, &wins, &losses, &queue) != 7) {
            destroyPlayerArray(array, n);
            fclose(f);
            return NULL;
        }

        Player* p = createPlayer(name);
        if (p == NULL) {
            destroyPlayerArray(array, n);
            fclose(f);
            return NULL;
        }
        p->level = level;
        p->spicyIndex = spicy;
        p->numGames = games;
        p->numWins = wins;
        p->numLosses = losses;
        p->inQueue = queue;
        array[i] = p;
    }

    fclose(f);
    *numPlayers = n;
    return array;
}

void savePlayers(const char* filename, Player** array, int size) {
    if (filename == NULL || array == NULL || size < 0) {
        return;
    }

    FILE* f = fopen(filename, "w");
    if (f == NULL) {
        printf("Erreur : impossible d'ecrire dans le fichier \"%s\".\n", filename);
        return;
    }

    int valid = 0;
    for (int i = 0; i < size; i++) {
        if (array[i] != NULL) {
            valid++;
        }
    }
    fprintf(f, "%d\n", valid);

    for (int i = 0; i < size; i++) {
        const Player* p = array[i];
        if (p == NULL) {
            continue;
        }
        fprintf(f, "%s %d %d %d %d %d %d\n", p->username, p->level, p->spicyIndex,
                p->numGames, p->numWins, p->numLosses, p->inQueue);
    }
    fclose(f);
}
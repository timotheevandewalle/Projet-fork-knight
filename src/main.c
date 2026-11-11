#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../headers/player.h"
#include "../headers/array-utils.h"
#include "../headers/list-utils.h"
#include "../headers/priority-queue.h"
#include "../headers/match.h"

/* ------------------------------------------------------------------ */
/* Utilitaires                                                         */
/* ------------------------------------------------------------------ */

// Lit une ligne au clavier et enleve le '\n' final
static void readLine(char *buffer, size_t size) {
    if (fgets(buffer, (int)size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
}

// Lit un pseudo avec un message d'invite
static void askName(const char *prompt, char *name, size_t size) {
    printf("%s", prompt);
    readLine(name, size);
}

static void printMenu(void) {
    printf("\n========================================\n");
    printf("       FORK KNIGHT - MATCHMAKER\n");
    printf("========================================\n");
    printf(" 1. Charger les joueurs depuis un fichier\n");
    printf(" 2. Sauvegarder les joueurs dans un fichier\n");
    printf(" 3. Ajouter un nouveau joueur\n");
    printf(" 4. Retirer un joueur\n");
    printf(" 5. Rechercher un joueur\n");
    printf(" 6. Afficher tous les joueurs\n");
    printf(" 7. Trier par spicyIndex\n");
    printf("----------------------------------------\n");
    printf(" 8. Ajouter un joueur a la file d'attente\n");
    printf(" 9. Retirer un joueur de la file d'attente\n");
    printf("10. Afficher la file d'attente\n");
    printf("11. Former un match (file d'attente)\n");
    printf("----------------------------------------\n");
    printf("12. Ajouter un joueur a la file de priorite\n");
    printf("13. Retirer le meilleur joueur (priorite)\n");
    printf("14. Afficher la file de priorite\n");
    printf("15. Former un match equilibre (priorite)\n");
    printf("----------------------------------------\n");
    printf("16. Mettre tous les joueurs libres dans la file d'attente\n");
    printf("17. Mettre tous les joueurs libres dans la file de priorite\n");
    printf(" 0. Quitter\n");
    printf("========================================\n");
    printf("Votre choix : ");
}

/* ------------------------------------------------------------------ */
/* Parties 1 et 2 : gestion des joueurs                                */
/* ------------------------------------------------------------------ */

static void loadMenu(Player **players, int *count) {
    char filename[128];
    askName("Fichier a charger (ex: input/players_50.txt) : ", filename, sizeof(filename));

    int n = 0;
    Player **loaded = loadPlayers(filename, &n);
    if (loaded == NULL) {
        printf("Impossible de charger \"%s\" (introuvable ou vide).\n", filename);
        return;
    }

    int added = 0;
    for (int i = 0; i < n; i++) {
        if (addPlayer(players, count, loaded[i])) {
            added++;
        } else {
            destroyPlayer(loaded[i]);   // doublon ou tableau plein
        }
    }
    free(loaded);   // seulement le tableau : les joueurs appartiennent a `players`

    printf("%d joueur(s) ajoute(s), %d ignore(s).\n", added, n - added);
}

static void saveMenu(Player **players, int count) {
    char filename[128];
    askName("Fichier de sortie (ex: output/players.txt) : ", filename, sizeof(filename));

    if (filename[0] == '\0') {
        printf("Nom de fichier vide, annule.\n");
        return;
    }

    savePlayers(filename, players, count);

    FILE *check = fopen(filename, "r");
    if (check == NULL) {
        printf("Echec de la sauvegarde (le dossier existe-t-il ?).\n");
    } else {
        fclose(check);
        printf("%d joueur(s) sauvegarde(s) dans \"%s\".\n", count, filename);
    }
}

static void addPlayerMenu(Player **players, int *count) {
    char name[64];
    askName("Pseudo du joueur (sans espace) : ", name, sizeof(name));

    if (name[0] == '\0') {
        printf("Pseudo vide, annule.\n");
        return;
    }
    if (strchr(name, ' ') != NULL) {
        printf("Le pseudo ne doit pas contenir d'espace (format de fichier).\n");
        return;
    }

    Player *p = createPlayer(name);
    if (p == NULL) {
        printf("Erreur lors de la creation du joueur.\n");
        return;
    }

    if (addPlayer(players, count, p)) {
        printf("Joueur \"%s\" ajoute (%d/%d).\n", p->username, *count, MAX_PLAYERS);
    } else {
        printf("Impossible d'ajouter \"%s\" (pseudo deja pris ou tableau plein).\n", p->username);
        destroyPlayer(p);
    }
}

static void removePlayerMenu(Player **players, int *count) {
    char name[64];
    askName("Pseudo a retirer : ", name, sizeof(name));

    int idx = searchPlayerByUsername(players, *count, name);
    if (idx == -1) {
        printf("Joueur \"%s\" introuvable.\n", name);
        return;
    }
    // removePlayer libere le joueur : s'il est dans une file, le pointeur deviendrait invalide
    if (players[idx]->inQueue) {
        printf("Joueur \"%s\" en file d'attente : retirez-le de la file d'abord.\n", name);
        return;
    }

    if (removePlayer(players, count, name)) {
        printf("Joueur \"%s\" retire.\n", name);
    } else {
        printf("Echec de la suppression.\n");
    }
}

static void searchMenu(Player **players, int count) {
    char name[64];
    askName("Pseudo a rechercher : ", name, sizeof(name));

    int idx = searchPlayerByUsername(players, count, name);
    if (idx == -1) {
        printf("Joueur \"%s\" introuvable.\n", name);
    } else {
        displayPlayer(players[idx]);
    }
}

/* ------------------------------------------------------------------ */
/* Parties 3 et 4 : files                                              */
/* ------------------------------------------------------------------ */

// Trouve un joueur libre (pas deja en file) a partir de son pseudo. NULL sinon.
static Player *findFreePlayer(Player **players, int count) {
    char name[64];
    askName("Pseudo du joueur : ", name, sizeof(name));

    int idx = searchPlayerByUsername(players, count, name);
    if (idx == -1) {
        printf("Joueur \"%s\" introuvable.\n", name);
        return NULL;
    }
    if (players[idx]->inQueue) {
        printf("Joueur \"%s\" deja en file d'attente.\n", name);
        return NULL;
    }
    return players[idx];
}

static void enqueueMenu(Player **players, int count, t_queue *queue) {
    Player *p = findFreePlayer(players, count);
    if (p != NULL) {
        enqueue(queue, p);
        printf("Joueur \"%s\" ajoute a la file d'attente.\n", p->username);
    }
}

static void dequeueMenu(t_queue *queue) {
    Player *p = dequeue(queue);
    if (p == NULL) {
        printf("La file d'attente est vide.\n");
    } else {
        printf("Joueur retire de la file : ");
        displayPlayerMini(p);
    }
}

static void insertPriorityMenu(Player **players, int count, t_priority_queue *pq) {
    Player *p = findFreePlayer(players, count);
    if (p != NULL) {
        insertWithPriority(pq, p);
        printf("Joueur \"%s\" ajoute a la file de priorite.\n", p->username);
    }
}

static void removeBestMenu(t_priority_queue *pq) {
    Player *p = removeHighestPriority(pq);
    if (p == NULL) {
        printf("La file de priorite est vide.\n");
    } else {
        printf("Meilleur joueur retire : ");
        displayPlayerMini(p);
    }
}

// Met tous les joueurs libres dans la file FIFO
static void fillQueue(Player **players, int count, t_queue *queue) {
    int added = 0;
    for (int i = 0; i < count; i++) {
        if (!players[i]->inQueue) {
            enqueue(queue, players[i]);
            added++;
        }
    }
    printf("%d joueur(s) ajoute(s) a la file d'attente.\n", added);
}

// Met tous les joueurs libres dans la file de priorite
static void fillPriorityQueue(Player **players, int count, t_priority_queue *pq) {
    int added = 0;
    for (int i = 0; i < count; i++) {
        if (!players[i]->inQueue) {
            insertWithPriority(pq, players[i]);
            added++;
        }
    }
    printf("%d joueur(s) ajoute(s) a la file de priorite.\n", added);
}

/* ------------------------------------------------------------------ */
/* Programme principal                                                 */
/* ------------------------------------------------------------------ */

int main(void) {
    srand((unsigned int)time(NULL));   // graine aleatoire pour les matchs (ici, pas dans simulateMatch)

    Player **players = createPlayerArray(MAX_PLAYERS);
    t_queue *queue = createQueue();
    t_priority_queue *pq = createPriorityQueue();

    if (players == NULL || queue == NULL || pq == NULL) {
        printf("Erreur d'allocation.\n");
        destroyPriorityQueue(pq);
        destroyQueue(queue);
        destroyPlayerArray(players, MAX_PLAYERS);
        return 1;
    }

    int count = 0;
    char input[64];
    int running = 1;

    while (running) {
        printMenu();
        readLine(input, sizeof(input));

        switch (atoi(input)) {
            case 1:  loadMenu(players, &count); break;
            case 2:  saveMenu(players, count); break;
            case 3:  addPlayerMenu(players, &count); break;
            case 4:  removePlayerMenu(players, &count); break;
            case 5:  searchMenu(players, count); break;
            case 6:  displayAllPlayers(players, count); break;
            case 7:
                sortPlayersBySpicyIndex(players, count);
                displayAllPlayers(players, count);
                break;
            case 8:  enqueueMenu(players, count, queue); break;
            case 9:  dequeueMenu(queue); break;
            case 10: displayQueue(queue); break;
            case 11: launchMatch(queue); break;
            case 12: insertPriorityMenu(players, count, pq); break;
            case 13: removeBestMenu(pq); break;
            case 14: displayPriorityQueue(pq); break;
            case 15: launchBalancedMatch(pq); break;
            case 16: fillQueue(players, count, queue); break;
            case 17: fillPriorityQueue(players, count, pq); break;
            case 0:  running = 0; break;
            default: printf("Choix invalide.\n");
        }
    }

    // Les files ne possedent pas les joueurs : on les detruit d'abord, puis le tableau
    destroyPriorityQueue(pq);
    destroyQueue(queue);
    destroyPlayerArray(players, MAX_PLAYERS);

    printf("Au revoir !\n");
    return 0;
}
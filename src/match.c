#include <stdio.h>
#include <stdlib.h>
#include "../headers/match.h"

Match *createMatch(void) {
    Match *match = malloc(sizeof(Match));
    if (match == NULL) {
        return NULL;
    }

    match->players = malloc(MATCH_MAX_PLAYERS * sizeof(Player *));
    if (match->players == NULL) {
        free(match);
        return NULL;
    }

    match->results = NULL;
    match->numPlayers = 0;
    return match;
}

// Ne libere pas les joueurs : ils appartiennent au tableau principal
void destroyMatch(Match *match) {
    if (match == NULL) {
        return;
    }
    free(match->players);
    free(match->results);   // free(NULL) est sans danger
    free(match);
}

void addMatchPlayers(t_queue *queue, Match *match) {
    if (queue == NULL || match == NULL) {
        return;
    }
    while (!isQueueEmpty(queue) && match->numPlayers < MATCH_MAX_PLAYERS) {
        Player *p = dequeue(queue);   // dequeue remet inQueue a 0
        if (p == NULL) {
            break;
        }
        match->players[match->numPlayers] = p;
        match->numPlayers++;
    }
}

// Melange de Fisher-Yates : permutation uniforme
static void fisherYatesShuffle(int *array, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
    }
}

// Remplit results avec une permutation aleatoire de 0..numPlayers-1
void simulateMatch(Match *match) {
    if (match == NULL || match->numPlayers <= 0) {
        return;
    }

    free(match->results);   // si le match est simule plusieurs fois
    match->results = malloc((size_t)match->numPlayers * sizeof(int));
    if (match->results == NULL) {
        return;
    }

    for (int i = 0; i < match->numPlayers; i++) {
        match->results[i] = i;
    }
    fisherYatesShuffle(match->results, match->numPlayers);
}

// gain au rang r = (n - r - 1) * (100 / (n - 1))
static int rankGain(int n, int r) {
    if (n <= 1) {
        return 0;   // evite la division par zero
    }
    return (n - r - 1) * (100 / (n - 1));
}

void updatePlayerStats(Match *match) {
    if (match == NULL || match->results == NULL) {
        return;
    }

    int n = match->numPlayers;
    for (int r = 0; r < n; r++) {
        Player *p = match->players[match->results[r]];
        p->numGames++;
        if (r == 0) {
            p->numWins++;
        } else {
            p->numLosses++;
        }
        p->spicyIndex += rankGain(n, r);
    }
}

void displayMatchInfo(const Match *match) {
    if (match == NULL) {
        printf("Erreur : match NULL.\n");
        return;
    }

    printf("\n========================================\n");
    printf("           MATCH INFORMATION\n");
    printf("========================================\n");
    printf("Participants: %d\n", match->numPlayers);
    printf("----------------------------------------\n");
    for (int i = 0; i < match->numPlayers; i++) {
        printf("%d. ", i + 1);
        displayPlayerMini(match->players[i]);
    }
    printf("========================================\n");
}

void displayMatchResult(const Match *match) {
    if (match == NULL || match->results == NULL) {
        printf("Erreur : match ou resultats NULL.\n");
        return;
    }

    int n = match->numPlayers;
    printf("\n========================================\n");
    printf("           MATCH RESULTS\n");
    printf("========================================\n");
    for (int r = 0; r < n; r++) {
        const Player *p = match->players[match->results[r]];
        printf("%d. [%s | Lvl: %d | Spicy: %d]  (+%d)\n",
               r + 1, p->username, p->level, p->spicyIndex, rankGain(n, r));
    }
    printf("========================================\n");
}

// Etapes communes : affichage, simulation, stats, resultats
static void playMatch(Match *match) {
    displayMatchInfo(match);

    simulateMatch(match);
    if (match->results == NULL) {
        printf("Erreur : la simulation du match a echoue.\n");
        return;
    }

    updatePlayerStats(match);
    displayMatchResult(match);
}

void launchMatch(t_queue *queue) {
    Match *match = createMatch();
    if (match == NULL) {
        printf("Erreur d'allocation du match.\n");
        return;
    }

    addMatchPlayers(queue, match);
    if (match->numPlayers == 0) {
        printf("Aucun joueur dans la file : match impossible.\n");
    } else {
        playMatch(match);
    }

    destroyMatch(match);
}

void launchBalancedMatch(t_priority_queue *p_queue) {
    Match *match = createMatch();
    if (match == NULL) {
        printf("Erreur d'allocation du match.\n");
        return;
    }

    // Les joueurs sortent du plus fort au plus faible : ecarts de spicyIndex minimaux
    while (match->numPlayers < MATCH_MAX_PLAYERS) {
        Player *p = removeHighestPriority(p_queue);
        if (p == NULL) {
            break;
        }
        match->players[match->numPlayers] = p;
        match->numPlayers++;
    }

    if (match->numPlayers == 0) {
        printf("Aucun joueur dans la file de priorite : match impossible.\n");
    } else {
        playMatch(match);
    }

    destroyMatch(match);
}
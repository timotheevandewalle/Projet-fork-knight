#ifndef MATCH_H
#define MATCH_H

#include "player.h"
#include "list-utils.h"
#include "priority-queue.h"

#define MATCH_MAX_PLAYERS 12

typedef struct {
    Player **players;   // participants
    int *results;       // results[r] = index du joueur classe r (0 = vainqueur)
    int numPlayers;     // nombre de joueurs (<= 12)
} Match;

Match *createMatch(void);
void destroyMatch(Match *match);
void addMatchPlayers(t_queue *queue, Match *match);
void simulateMatch(Match *match);
void updatePlayerStats(Match *match);
void displayMatchInfo(const Match *match);
void displayMatchResult(const Match *match);
void launchMatch(t_queue *queue);
void launchBalancedMatch(t_priority_queue *p_queue);

#endif // MATCH_H
#ifndef LIST_UTILS_H
#define LIST_UTILS_H

#include "player.h"

/* Cellule de la liste chainee : stocke l'adresse d'un joueur */
typedef struct s_cell {
    Player *value;
    struct s_cell *next;
} t_cell;

/* File d'attente (FIFO) : liste chainee avec tete et queue */
typedef struct s_queue {
    t_cell *head;
    t_cell *tail;
} t_queue;

t_cell *createCell(Player *player);
void destroyCell(t_cell *cell);

t_queue *createQueue(void);
int isQueueEmpty(t_queue *queue);
void enqueue(t_queue *queue, Player *player);
Player *dequeue(t_queue *queue);
void displayQueue(t_queue *queue);

t_cell *removeHead(t_queue *queue);
void destroyQueue(t_queue *queue);

#endif // LIST_UTILS_H
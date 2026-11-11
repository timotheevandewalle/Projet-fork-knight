#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "player.h"
#include "list-utils.h"

/* File de priorite : meme structure que la file FIFO, mais triee par spicyIndex */
typedef t_queue t_priority_queue;

t_priority_queue *createPriorityQueue(void);
void insertWithPriority(t_priority_queue *queue, Player *player);
Player *removeHighestPriority(t_priority_queue *queue);
void displayPriorityQueue(t_priority_queue *queue);
void destroyPriorityQueue(t_priority_queue *queue);

#endif // PRIORITY_QUEUE_H
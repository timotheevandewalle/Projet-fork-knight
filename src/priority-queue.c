#include <stdio.h>
#include <stdlib.h>
#include "../headers/priority-queue.h"

t_priority_queue *createPriorityQueue(void) {
    return createQueue();
}

// Insertion triee par spicyIndex decroissant.
// A egalite, le nouvel arrivant est place apres les joueurs deja presents.
void insertWithPriority(t_priority_queue *queue, Player *player) {
    if (queue == NULL || player == NULL) {
        return;
    }

    t_cell *cell = createCell(player);
    if (cell == NULL) {
        return;
    }

    // Cas 1 et 2 : file vide, ou nouveau joueur strictement plus fort que la tete
    if (queue->head == NULL || player->spicyIndex > queue->head->value->spicyIndex) {
        cell->next = queue->head;
        queue->head = cell;
        if (queue->tail == NULL) {
            queue->tail = cell;
        }
    } else {
        // Cas 3 et 4 : on avance tant que le suivant a un spicyIndex >= au nouveau
        t_cell *cur = queue->head;
        while (cur->next != NULL && cur->next->value->spicyIndex >= player->spicyIndex) {
            cur = cur->next;
        }
        cell->next = cur->next;
        cur->next = cell;
        if (cell->next == NULL) {   // insertion en fin de file
            queue->tail = cell;
        }
    }

    player->inQueue = 1;
}

Player *removeHighestPriority(t_priority_queue *queue) {
    return dequeue(queue);
}

void displayPriorityQueue(t_priority_queue *queue) {
    printf("\n====== File de Priorite ======\n");
    if (queue == NULL) {
        printf("File invalide.\n");
        printf("==============================\n");
        return;
    }

    int n = 0;
    for (t_cell *cur = queue->head; cur != NULL; cur = cur->next) {
        n++;
        printf("%d. [%s | Lvl: %d | Spicy: %d]%s\n",
               n, cur->value->username, cur->value->level, cur->value->spicyIndex,
               (n == 1) ? " (max)" : "");
    }
    printf("Total: %d joueur%s (ordre decroissant)\n", n, (n > 1) ? "s" : "");
    printf("==============================\n");
}

void destroyPriorityQueue(t_priority_queue *queue) {
    destroyQueue(queue);
}
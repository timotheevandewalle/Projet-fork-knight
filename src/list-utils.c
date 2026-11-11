#include <stdio.h>
#include <stdlib.h>
#include "../headers/list-utils.h"

// Cree une cellule contenant l'adresse du joueur (le joueur n'est pas copie)
t_cell *createCell(Player *player) {
    t_cell *cell = malloc(sizeof(t_cell));
    if (cell == NULL) {
        return NULL;
    }
    cell->value = player;
    cell->next = NULL;
    return cell;
}

// Libere la cellule uniquement, jamais le joueur
void destroyCell(t_cell *cell) {
    if (cell == NULL) {
        return;
    }
    free(cell);
}

t_queue *createQueue(void) {
    t_queue *queue = malloc(sizeof(t_queue));
    if (queue == NULL) {
        return NULL;
    }
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

int isQueueEmpty(t_queue *queue) {
    return (queue == NULL || queue->head == NULL) ? 1 : 0;
}

// Ajoute le joueur en fin de file
void enqueue(t_queue *queue, Player *player) {
    if (queue == NULL || player == NULL) {
        return;
    }

    t_cell *cell = createCell(player);
    if (cell == NULL) {
        return;
    }

    if (queue->tail == NULL) {      // file vide : la cellule est tete et queue
        queue->head = cell;
    } else {
        queue->tail->next = cell;
    }
    queue->tail = cell;
    player->inQueue = 1;
}

// Retire la premiere cellule sans la liberer (utile pour destroyQueue)
t_cell *removeHead(t_queue *queue) {
    if (isQueueEmpty(queue)) {
        return NULL;
    }

    t_cell *cell = queue->head;
    queue->head = cell->next;
    if (queue->head == NULL) {      // dernier element retire : tail ne doit plus pointer dessus
        queue->tail = NULL;
    }
    cell->next = NULL;

    if (cell->value != NULL) {
        cell->value->inQueue = 0;
    }
    return cell;
}

// Retire et retourne le premier joueur (la cellule est liberee, pas le joueur)
Player *dequeue(t_queue *queue) {
    t_cell *cell = removeHead(queue);
    if (cell == NULL) {
        return NULL;
    }
    Player *player = cell->value;
    destroyCell(cell);
    return player;
}

void displayQueue(t_queue *queue) {
    printf("\n======= File d'attente =======\n");
    if (queue == NULL) {
        printf("File invalide.\n");
        printf("==============================\n");
        return;
    }

    int n = 0;
    for (t_cell *cur = queue->head; cur != NULL; cur = cur->next) {
        n++;
        printf("%d. ", n);
        displayPlayerMini(cur->value);
    }
    printf("Total: %d joueur%s en attente\n", n, (n > 1) ? "s" : "");
    printf("==============================\n");
}

// Libere les cellules et la file, mais pas les joueurs
void destroyQueue(t_queue *queue) {
    if (queue == NULL) {
        return;
    }
    t_cell *cell;
    while ((cell = removeHead(queue)) != NULL) {
        destroyCell(cell);
    }
    free(queue);
}
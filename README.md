# Fork Knight - Matchmaking

Simulateur de matchmaking en C pour un jeu fictif, réalisé en binôme dans le cadre du module de programmation à l'EFREI Paris.

## Fonctionnalités

- Gestion des joueurs : création, ajout, suppression, recherche par pseudo, tri par Spicy Index
- Tableau dynamique et sauvegarde/chargement des joueurs dans des fichiers texte
- File d'attente FIFO (liste chaînée)
- File de priorité triée par Spicy Index pour des matchs équilibrés
- Matchs de 12 joueurs maximum : simulation aléatoire (Fisher-Yates) et mise à jour des statistiques
- Menu interactif en console

## Structure

- `headers/` et `src/` : code source (`player`, `array-utils`, `list-utils`, `priority-queue`, `match`)
- `input/` et `output/` : fichiers de joueurs en entrée et sauvegardes
- `tests/` : tests unitaires (framework Unity)

## Utilisation de l'IA générative

Une IA générative (Claude) a été utilisée pour [préciser les parties concernées : par exemple le menu de `main.c`, les fonctions de `list-utils.c`, `priority-queue.c` et `match.c`, la correction de `array-utils.c`]. Le code a été relu et compris par les membres du binôme.
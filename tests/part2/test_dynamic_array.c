/**
 * Tests unitaires pour la Partie 2 : Gestion dynamique des joueurs
 * Framework: Unity
 * 
 * Fonctions testées:
 * - createPlayerArray()
 * - destroyPlayerArray()
 * - loadPlayers()
 * - savePlayers()
 * 
 * Tests de non-régression (Partie 1 avec tableau dynamique):
 * - addPlayer()
 * - searchPlayerByUsername()
 * - removePlayer()
 * - sortPlayersBySpicyIndex()
 * - displayAllPlayers()
 */

#include "../unity/unity.h"
#include "../../headers/player.h"
#include "../../headers/array-utils.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Fonction de setup Unity (appelée avant chaque test)
void setUp(void) {
    // Rien à faire ici pour l'instant
}

// Fonction de teardown Unity (appelée après chaque test)
void tearDown(void) {
    // Rien à faire ici pour l'instant
}

// ============================================================================
// Tests pour createPlayerArray
// ============================================================================

void test_createPlayerArray_should_allocate_array(void) {
    int size = 10;
    Player** players = createPlayerArray(size);
    
    TEST_ASSERT_NOT_NULL(players);
    
    // Vérifier que tous les pointeurs sont initialisés à NULL
    for (int i = 0; i < size; i++) {
        TEST_ASSERT_NULL(players[i]);
    }
    
    free(players);
}

void test_createPlayerArray_should_handle_zero_size(void) {
    int size = 0;
    Player** players = createPlayerArray(size);
    
    // Peut retourner NULL ou un tableau vide selon l'implémentation
    if (players != NULL) {
        free(players);
    }
    
    TEST_ASSERT_TRUE(1); // Pass if no crash
}

void test_createPlayerArray_should_handle_large_size(void) {
    int size = 1000;
    Player** players = createPlayerArray(size);
    
    TEST_ASSERT_NOT_NULL(players);
    
    free(players);
}

// ============================================================================
// Tests pour destroyPlayerArray
// ============================================================================

void test_destroyPlayerArray_should_free_all_players(void) {
    int size = 5;
    Player** players = createPlayerArray(size);
    
    // Ajouter quelques joueurs
    players[0] = createPlayer("Alice");
    players[1] = createPlayer("Bob");
    players[2] = createPlayer("Charlie");
    
    // destroyPlayerArray devrait libérer tous les joueurs ET le tableau
    destroyPlayerArray(players, size);
    
    // Test réussi si pas de crash ni de fuite mémoire (vérifier avec Valgrind)
    TEST_ASSERT_TRUE(1);
}

void test_destroyPlayerArray_should_handle_empty_array(void) {
    int size = 10;
    Player** players = createPlayerArray(size);
    
    // Détruire un tableau avec des NULL ne devrait pas crasher
    destroyPlayerArray(players, size);
    
    TEST_ASSERT_TRUE(1);
}

void test_destroyPlayerArray_should_handle_partial_array(void) {
    int size = 5;
    Player** players = createPlayerArray(size);
    
    // Ajouter seulement 2 joueurs sur 5
    players[0] = createPlayer("Alice");
    players[2] = createPlayer("Bob");
    // players[1], [3], [4] restent à NULL
    
    destroyPlayerArray(players, size);
    
    TEST_ASSERT_TRUE(1);
}

// ============================================================================
// Tests pour loadPlayers et savePlayers
// ============================================================================

void test_savePlayers_should_create_file_with_correct_format(void) {
    // Créer un tableau de joueurs
    int size = 2;
    Player** players = createPlayerArray(size);
    
    players[0] = createPlayer("Alice");
    players[0]->level = 5;
    players[0]->spicyIndex = 1000;
    players[0]->numGames = 5;
    players[0]->numWins = 3;
    players[0]->numLosses = 2;
    
    players[1] = createPlayer("Bob");
    players[1]->level = 10;
    players[1]->spicyIndex = 1500;
    players[1]->numGames = 0;
    players[1]->numWins = 0;
    players[1]->numLosses = 0;
    
    // Sauvegarder dans un fichier
    savePlayers("output/test_save.txt", players, size);
    
    // Vérifier que le fichier existe
    FILE* f = fopen("output/test_save.txt", "r");
    TEST_ASSERT_NOT_NULL(f);
    
    // Lire la première ligne (nombre de joueurs)
    int savedCount;
    fscanf(f, "%d", &savedCount);
    TEST_ASSERT_EQUAL_INT(2, savedCount);
    
    fclose(f);
    destroyPlayerArray(players, size);
    unlink("output/test_save.txt");  // Nettoyer
}

void test_loadPlayers_should_load_from_file(void) {
    // Créer un fichier de test
    FILE* f = fopen("output/test_load.txt", "w");
    fprintf(f, "2\n");
    fprintf(f, "Alice 5 1000 10 7 3 0\n");
    fprintf(f, "Bob 10 1500 5 4 1 0\n");
    fclose(f);
    
    // Charger les joueurs
    int numPlayers = 0;
    Player** players = loadPlayers("output/test_load.txt", &numPlayers);
    
    TEST_ASSERT_NOT_NULL(players);
    TEST_ASSERT_EQUAL_INT(2, numPlayers);
    
    // Vérifier les données
    TEST_ASSERT_EQUAL_STRING("Alice", players[0]->username);
    TEST_ASSERT_EQUAL_INT(5, players[0]->level);
    TEST_ASSERT_EQUAL_INT(1000, players[0]->spicyIndex);
    TEST_ASSERT_EQUAL_INT(10, players[0]->numGames);
    TEST_ASSERT_EQUAL_INT(7, players[0]->numWins);
    
    TEST_ASSERT_EQUAL_STRING("Bob", players[1]->username);
    TEST_ASSERT_EQUAL_INT(10, players[1]->level);
    
    destroyPlayerArray(players, numPlayers);
    unlink("output/test_load.txt");  // Nettoyer
}

void test_loadPlayers_should_return_NULL_if_file_not_found(void) {
    int numPlayers = 0;
    Player** players = loadPlayers("output/nonexistent_file.txt", &numPlayers);
    
    TEST_ASSERT_NULL(players);
    TEST_ASSERT_EQUAL_INT(0, numPlayers);
}

void test_loadPlayers_should_handle_empty_file(void) {
    // Créer un fichier avec 0 joueurs
    FILE* f = fopen("output/test_empty.txt", "w");
    fprintf(f, "0\n");
    fclose(f);
    
    int numPlayers = 0;
    Player** players = loadPlayers("output/test_empty.txt", &numPlayers);
    
    TEST_ASSERT_EQUAL_INT(0, numPlayers);
    
    // Peut retourner NULL ou un tableau vide selon l'implémentation
    if (players != NULL) {
        free(players);
    }
    
    unlink("output/test_empty.txt");
}

void test_save_and_load_round_trip(void) {
    // Créer et sauvegarder des joueurs
    int size = 5;
    Player** players = createPlayerArray(size);
    
    for (int i = 0; i < size; i++) {
        char name[20];
        snprintf(name, sizeof(name), "Player%d", i);
        players[i] = createPlayer(name);
        players[i]->level = i + 1;
        players[i]->spicyIndex = 1000 + i * 100;
        players[i]->numGames = i * 2;
        players[i]->numWins = i;
        players[i]->numLosses = i;
    }
    
    savePlayers("output/test_roundtrip.txt", players, size);
    destroyPlayerArray(players, size);
    
    // Recharger
    int numPlayers = 0;
    players = loadPlayers("output/test_roundtrip.txt", &numPlayers);
    
    TEST_ASSERT_NOT_NULL(players);
    TEST_ASSERT_EQUAL_INT(5, numPlayers);
    TEST_ASSERT_EQUAL_STRING("Player0", players[0]->username);
    TEST_ASSERT_EQUAL_STRING("Player4", players[4]->username);
    TEST_ASSERT_EQUAL_INT(5, players[4]->level);
    TEST_ASSERT_EQUAL_INT(8, players[4]->numGames);
    
    destroyPlayerArray(players, numPlayers);
    unlink("output/test_roundtrip.txt");
}

// ============================================================================
// Tests supplémentaires: chargements depuis input/players_50.txt et players_100.txt
// ============================================================================

void test_loadPlayers_should_load_50_players_from_input_file(void) {
    int numPlayers = 0;
    Player** players = loadPlayers("input/players_50.txt", &numPlayers);

    TEST_ASSERT_NOT_NULL(players);
    TEST_ASSERT_EQUAL_INT(50, numPlayers);

    // Vérifier quelques valeurs (selon le dataset généré)
    TEST_ASSERT_EQUAL_STRING("Chef001", players[0]->username);
    TEST_ASSERT_EQUAL_INT(2, players[0]->level);
    TEST_ASSERT_EQUAL_INT(610, players[0]->spicyIndex);
    TEST_ASSERT_EQUAL_INT(3, players[0]->numGames);
    TEST_ASSERT_EQUAL_INT(1, players[0]->numWins);
    TEST_ASSERT_EQUAL_INT(2, players[0]->numLosses);

    TEST_ASSERT_EQUAL_STRING("Chef050", players[49]->username);
    TEST_ASSERT_EQUAL_INT(11, players[49]->level);
    TEST_ASSERT_EQUAL_INT(1100, players[49]->spicyIndex);
    TEST_ASSERT_EQUAL_INT(10, players[49]->numGames);
    TEST_ASSERT_EQUAL_INT(5, players[49]->numWins);
    TEST_ASSERT_EQUAL_INT(5, players[49]->numLosses);

    destroyPlayerArray(players, numPlayers);
}

void test_loadPlayers_should_load_100_players_from_input_file(void) {
    int numPlayers = 0;
    Player** players = loadPlayers("input/players_100.txt", &numPlayers);

    TEST_ASSERT_NOT_NULL(players);
    TEST_ASSERT_EQUAL_INT(100, numPlayers);

    // Vérifier quelques valeurs
    TEST_ASSERT_EQUAL_STRING("Chef001", players[0]->username);
    TEST_ASSERT_EQUAL_INT(2, players[0]->level);
    TEST_ASSERT_EQUAL_INT(610, players[0]->spicyIndex);

    TEST_ASSERT_EQUAL_STRING("Chef100", players[99]->username);
    TEST_ASSERT_EQUAL_INT(1, players[99]->level);
    TEST_ASSERT_EQUAL_INT(1600, players[99]->spicyIndex);
    TEST_ASSERT_EQUAL_INT(0, players[99]->numGames);

    destroyPlayerArray(players, numPlayers);
}

void test_savePlayers_round_trip_with_100_from_input(void) {
    int numPlayers = 0;
    Player** players = loadPlayers("input/players_100.txt", &numPlayers);

    TEST_ASSERT_NOT_NULL(players);
    TEST_ASSERT_EQUAL_INT(100, numPlayers);

    // Sauvegarder puis recharger
    const char* outpath = "output/players_100_copy.txt";
    savePlayers(outpath, players, numPlayers);

    destroyPlayerArray(players, numPlayers);

    int reloadedCount = 0;
    players = loadPlayers(outpath, &reloadedCount);

    TEST_ASSERT_NOT_NULL(players);
    TEST_ASSERT_EQUAL_INT(100, reloadedCount);
    TEST_ASSERT_EQUAL_STRING("Chef100", players[99]->username);
    TEST_ASSERT_EQUAL_INT(1600, players[99]->spicyIndex);

    destroyPlayerArray(players, reloadedCount);
    unlink(outpath);
}

// ============================================================================
// Tests de non-régression : Partie 1 avec tableau dynamique
// ============================================================================

void test_regression_addPlayer_should_work_with_dynamic_array(void) {
    int size = 10;
    Player** players = createPlayerArray(size);
    int count = 0;
    
    // Ajouter des joueurs avec addPlayer (fonction Partie 1)
    Player* p1 = createPlayer("Alice");
    p1->level = 5;
    p1->spicyIndex = 1000;
    
    Player* p2 = createPlayer("Bob");
    p2->level = 10;
    p2->spicyIndex = 1500;
    
    int result1 = addPlayer(players, &count, p1);
    int result2 = addPlayer(players, &count, p2);
    
    TEST_ASSERT_EQUAL_INT(1, result1);
    TEST_ASSERT_EQUAL_INT(1, result2);
    TEST_ASSERT_EQUAL_INT(2, count);
    TEST_ASSERT_EQUAL_PTR(p1, players[0]);
    TEST_ASSERT_EQUAL_PTR(p2, players[1]);
    
    destroyPlayerArray(players, size);
}

void test_regression_searchPlayer_should_work_with_dynamic_array(void) {
    int size = 5;
    Player** players = createPlayerArray(size);
    int count = 0;
    
    // Ajouter quelques joueurs
    Player* p1 = createPlayer("Alice");
    Player* p2 = createPlayer("Bob");
    Player* p3 = createPlayer("Charlie");
    
    addPlayer(players, &count, p1);
    addPlayer(players, &count, p2);
    addPlayer(players, &count, p3);
    
    // Rechercher avec searchPlayerByUsername (fonction Partie 1)
    int index_alice = searchPlayerByUsername(players, count, "Alice");
    int index_bob = searchPlayerByUsername(players, count, "Bob");
    int index_notfound = searchPlayerByUsername(players, count, "NotFound");
    
    TEST_ASSERT_EQUAL_INT(0, index_alice);
    TEST_ASSERT_EQUAL_INT(1, index_bob);
    TEST_ASSERT_EQUAL_INT(-1, index_notfound);
    
    destroyPlayerArray(players, size);
}

void test_regression_removePlayer_should_work_with_dynamic_array(void) {
    int size = 5;
    Player** players = createPlayerArray(size);
    int count = 0;
    
    // Ajouter des joueurs
    Player* p1 = createPlayer("Alice");
    Player* p2 = createPlayer("Bob");
    Player* p3 = createPlayer("Charlie");
    
    addPlayer(players, &count, p1);
    addPlayer(players, &count, p2);
    addPlayer(players, &count, p3);
    
    TEST_ASSERT_EQUAL_INT(3, count);
    
    // Retirer Bob avec removePlayer (fonction Partie 1)
    int result = removePlayer(players, &count, "Bob");
    
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_INT(2, count);
    
    // Vérifier que Bob n'est plus là
    int index = searchPlayerByUsername(players, count, "Bob");
    TEST_ASSERT_EQUAL_INT(-1, index);
    
    // Vérifier que Alice et Charlie sont toujours là
    TEST_ASSERT_EQUAL_STRING("Alice", players[0]->username);
    TEST_ASSERT_EQUAL_STRING("Charlie", players[1]->username);
    
    destroyPlayerArray(players, size);
}

void test_regression_sortPlayers_should_work_with_dynamic_array(void) {
    int size = 5;
    Player** players = createPlayerArray(size);
    int count = 0;
    
    // Ajouter des joueurs avec différents spicyIndex
    Player* p1 = createPlayer("Alice");
    p1->spicyIndex = 800;
    
    Player* p2 = createPlayer("Bob");
    p2->spicyIndex = 1500;
    
    Player* p3 = createPlayer("Charlie");
    p3->spicyIndex = 1200;
    
    Player* p4 = createPlayer("Dave");
    p4->spicyIndex = 1000;
    
    addPlayer(players, &count, p1);
    addPlayer(players, &count, p2);
    addPlayer(players, &count, p3);
    addPlayer(players, &count, p4);
    
    // Trier par spicyIndex décroissant (fonction Partie 1)
    sortPlayersBySpicyIndex(players, count);
    
    // Vérifier l'ordre : Bob(1500) > Charlie(1200) > Dave(1000) > Alice(800)
    TEST_ASSERT_EQUAL_STRING("Bob", players[0]->username);
    TEST_ASSERT_EQUAL_INT(1500, players[0]->spicyIndex);
    
    TEST_ASSERT_EQUAL_STRING("Charlie", players[1]->username);
    TEST_ASSERT_EQUAL_INT(1200, players[1]->spicyIndex);
    
    TEST_ASSERT_EQUAL_STRING("Dave", players[2]->username);
    TEST_ASSERT_EQUAL_INT(1000, players[2]->spicyIndex);
    
    TEST_ASSERT_EQUAL_STRING("Alice", players[3]->username);
    TEST_ASSERT_EQUAL_INT(800, players[3]->spicyIndex);
    
    destroyPlayerArray(players, size);
}

void test_regression_displayAllPlayers_should_work_with_dynamic_array(void) {
    int size = 3;
    Player** players = createPlayerArray(size);
    int count = 0;
    
    // Ajouter des joueurs
    Player* p1 = createPlayer("Alice");
    p1->level = 5;
    p1->spicyIndex = 1000;
    
    Player* p2 = createPlayer("Bob");
    p2->level = 10;
    p2->spicyIndex = 1500;
    
    addPlayer(players, &count, p1);
    addPlayer(players, &count, p2);
    
    // Appeler displayAllPlayers (fonction Partie 1) - ne devrait pas crasher
    displayAllPlayers(players, count);
    
    destroyPlayerArray(players, size);
    TEST_ASSERT_TRUE(1);
}

void test_regression_full_workflow_with_dynamic_array(void) {
    // Test d'intégration complet : charger, manipuler, sauvegarder
    int numPlayers = 0;
    
    // 1. Charger depuis un fichier
    Player** players = loadPlayers("input/player_10.txt", &numPlayers);
    TEST_ASSERT_NOT_NULL(players);
    TEST_ASSERT_EQUAL_INT(10, numPlayers);
    
    // 2. Ajouter un nouveau joueur (si capacité le permet)
    // Note: createPlayerArray crée un tableau de taille fixe,
    // donc on ne peut pas ajouter au-delà de numPlayers
    // Ce test vérifie juste que les fonctions ne crashent pas
    
    // 3. Rechercher un joueur existant
    int idx = searchPlayerByUsername(players, numPlayers, "ForkMaster");
    TEST_ASSERT_TRUE(idx >= 0);
    TEST_ASSERT_EQUAL_STRING("ForkMaster", players[idx]->username);
    
    // 4. Trier les joueurs
    sortPlayersBySpicyIndex(players, numPlayers);
    
    // Vérifier que le tri est correct (ordre décroissant)
    for (int i = 0; i < numPlayers - 1; i++) {
        TEST_ASSERT_TRUE(players[i]->spicyIndex >= players[i + 1]->spicyIndex);
    }
    
    // 5. Afficher tous les joueurs
    displayAllPlayers(players, numPlayers);
    
    // 6. Retirer un joueur
    int oldCount = numPlayers;
    int removed = removePlayer(players, &numPlayers, "RustyFork");
    TEST_ASSERT_EQUAL_INT(1, removed);
    TEST_ASSERT_EQUAL_INT(oldCount - 1, numPlayers);
    
    // 7. Sauvegarder
    savePlayers("output/test_workflow.txt", players, numPlayers);
    
    // 8. Nettoyer
    destroyPlayerArray(players, oldCount); // Utiliser la taille originale
    
    // 9. Recharger pour vérifier
    int reloadedCount = 0;
    players = loadPlayers("output/test_workflow.txt", &reloadedCount);
    TEST_ASSERT_NOT_NULL(players);
    TEST_ASSERT_EQUAL_INT(oldCount - 1, reloadedCount);
    
    // Vérifier que RustyFork n'est plus là
    int notFound = searchPlayerByUsername(players, reloadedCount, "RustyFork");
    TEST_ASSERT_EQUAL_INT(-1, notFound);
    
    destroyPlayerArray(players, reloadedCount);
    unlink("output/test_workflow.txt");
}

// ============================================================================
// Fonction principale
// ============================================================================

int main(void) {
    UNITY_BEGIN();
    
    // Tests createPlayerArray
    RUN_TEST(test_createPlayerArray_should_allocate_array);
    RUN_TEST(test_createPlayerArray_should_handle_zero_size);
    RUN_TEST(test_createPlayerArray_should_handle_large_size);
    
    // Tests destroyPlayerArray
    RUN_TEST(test_destroyPlayerArray_should_free_all_players);
    RUN_TEST(test_destroyPlayerArray_should_handle_empty_array);
    RUN_TEST(test_destroyPlayerArray_should_handle_partial_array);
    
    // Tests loadPlayers et savePlayers
    RUN_TEST(test_savePlayers_should_create_file_with_correct_format);
    RUN_TEST(test_loadPlayers_should_load_from_file);
    RUN_TEST(test_loadPlayers_should_return_NULL_if_file_not_found);
    RUN_TEST(test_loadPlayers_should_handle_empty_file);
    RUN_TEST(test_save_and_load_round_trip);
    RUN_TEST(test_loadPlayers_should_load_50_players_from_input_file);
    RUN_TEST(test_loadPlayers_should_load_100_players_from_input_file);
    RUN_TEST(test_savePlayers_round_trip_with_100_from_input);
    
    // Tests de non-régression : Partie 1 avec tableau dynamique
    RUN_TEST(test_regression_addPlayer_should_work_with_dynamic_array);
    RUN_TEST(test_regression_searchPlayer_should_work_with_dynamic_array);
    RUN_TEST(test_regression_removePlayer_should_work_with_dynamic_array);
    RUN_TEST(test_regression_sortPlayers_should_work_with_dynamic_array);
    RUN_TEST(test_regression_displayAllPlayers_should_work_with_dynamic_array);
    RUN_TEST(test_regression_full_workflow_with_dynamic_array);
    
    return UNITY_END();
}

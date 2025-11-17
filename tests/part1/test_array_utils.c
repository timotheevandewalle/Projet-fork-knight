/**
 * Tests unitaires - Partie 1
 * Module: array-utils.c
 */

#include "../unity/unity.h"
#include "../../headers/player.h"
#include "../../headers/array-utils.h"

void setUp(void) {}
void tearDown(void) {}

// ========================================
// Tests pour addPlayer()
// ========================================

void test_addPlayer_should_add_to_empty_array(void) {
    Player* players[MAX_PLAYERS];
    int count = 0;
    
    Player* p = createPlayer("Player1");
    TEST_ASSERT_NOT_NULL(p);
    
    int result = addPlayer(players, &count, p);
    
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_INT(1, count);
    TEST_ASSERT_EQUAL_PTR(p, players[0]);
    
    destroyPlayer(p);
}

void test_addPlayer_should_add_multiple_players(void) {
    Player* players[MAX_PLAYERS];
    int count = 0;
    
    Player* p1 = createPlayer("Player1");
    Player* p2 = createPlayer("Player2");
    Player* p3 = createPlayer("Player3");
    
    TEST_ASSERT_EQUAL_INT(1, addPlayer(players, &count, p1));
    TEST_ASSERT_EQUAL_INT(1, addPlayer(players, &count, p2));
    TEST_ASSERT_EQUAL_INT(1, addPlayer(players, &count, p3));
    
    TEST_ASSERT_EQUAL_INT(3, count);
    TEST_ASSERT_EQUAL_PTR(p1, players[0]);
    TEST_ASSERT_EQUAL_PTR(p2, players[1]);
    TEST_ASSERT_EQUAL_PTR(p3, players[2]);
    
    destroyPlayer(p1);
    destroyPlayer(p2);
    destroyPlayer(p3);
}

void test_addPlayer_should_respect_max_capacity(void) {
    Player* players[MAX_PLAYERS];
    int count = MAX_PLAYERS;  // Déjà plein
    
    Player* p = createPlayer("Overflow");
    
    int result = addPlayer(players, &count, p);
    
    TEST_ASSERT_EQUAL_INT(0, result);  // Doit échouer
    TEST_ASSERT_EQUAL_INT(MAX_PLAYERS, count);  // Count inchangé
    
    destroyPlayer(p);
}

// ========================================
// Tests pour searchPlayerByUsername()
// ========================================

void test_searchPlayer_should_find_existing_player(void) {
    Player* players[MAX_PLAYERS];
    int count = 0;
    
    Player* p1 = createPlayer("Alice");
    Player* p2 = createPlayer("Bob");
    Player* p3 = createPlayer("Charlie");
    
    addPlayer(players, &count, p1);
    addPlayer(players, &count, p2);
    addPlayer(players, &count, p3);
    
    int index = searchPlayerByUsername(players, count, "Bob");
    
    TEST_ASSERT_EQUAL_INT(1, index);
    TEST_ASSERT_EQUAL_STRING("Bob", players[index]->username);
    
    destroyPlayer(p1);
    destroyPlayer(p2);
    destroyPlayer(p3);
}

void test_searchPlayer_should_return_minus_one_if_not_found(void) {
    Player* players[MAX_PLAYERS];
    int count = 0;
    
    Player* p1 = createPlayer("Alice");
    addPlayer(players, &count, p1);
    
    int index = searchPlayerByUsername(players, count, "Unknown");
    
    TEST_ASSERT_EQUAL_INT(-1, index);
    
    destroyPlayer(p1);
}

void test_searchPlayer_should_return_minus_one_on_empty_array(void) {
    Player* players[MAX_PLAYERS];
    int count = 0;
    
    int index = searchPlayerByUsername(players, count, "Anyone");
    
    TEST_ASSERT_EQUAL_INT(-1, index);
}

void test_searchPlayer_should_find_first_player(void) {
    Player* players[MAX_PLAYERS];
    int count = 0;
    
    Player* p1 = createPlayer("First");
    Player* p2 = createPlayer("Second");
    
    addPlayer(players, &count, p1);
    addPlayer(players, &count, p2);
    
    int index = searchPlayerByUsername(players, count, "First");
    
    TEST_ASSERT_EQUAL_INT(0, index);
    
    destroyPlayer(p1);
    destroyPlayer(p2);
}

void test_searchPlayer_should_find_last_player(void) {
    Player* players[MAX_PLAYERS];
    int count = 0;
    
    Player* p1 = createPlayer("First");
    Player* p2 = createPlayer("Last");
    
    addPlayer(players, &count, p1);
    addPlayer(players, &count, p2);
    
    int index = searchPlayerByUsername(players, count, "Last");
    
    TEST_ASSERT_EQUAL_INT(1, index);
    
    destroyPlayer(p1);
    destroyPlayer(p2);
}

// ========================================
// Tests pour removePlayer()
// ========================================

void test_removePlayer_should_remove_from_middle(void) {
    Player* players[MAX_PLAYERS];
    int count = 0;
    
    Player* p1 = createPlayer("Alice");
    Player* p2 = createPlayer("Bob");
    Player* p3 = createPlayer("Charlie");
    
    addPlayer(players, &count, p1);
    addPlayer(players, &count, p2);
    addPlayer(players, &count, p3);
    
    // Sauvegarder les pointeurs avant suppression
    Player* saved_p1 = p1;
    Player* saved_p3 = p3;
    
    int result = removePlayer(players, &count, "Bob");
    
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_INT(2, count);
    
    // Vérifier le décalage
    TEST_ASSERT_EQUAL_PTR(saved_p1, players[0]);
    TEST_ASSERT_EQUAL_PTR(saved_p3, players[1]);
    
    // p2 a été libéré par removePlayer
    destroyPlayer(p1);
    destroyPlayer(p3);
}

void test_removePlayer_should_remove_first(void) {
    Player* players[MAX_PLAYERS];
    int count = 0;
    
    Player* p1 = createPlayer("First");
    Player* p2 = createPlayer("Second");
    
    addPlayer(players, &count, p1);
    addPlayer(players, &count, p2);
    
    Player* saved_p2 = p2;
    
    int result = removePlayer(players, &count, "First");
    
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_INT(1, count);
    TEST_ASSERT_EQUAL_PTR(saved_p2, players[0]);
    
    destroyPlayer(p2);
}

void test_removePlayer_should_remove_last(void) {
    Player* players[MAX_PLAYERS];
    int count = 0;
    
    Player* p1 = createPlayer("First");
    Player* p2 = createPlayer("Last");
    
    addPlayer(players, &count, p1);
    addPlayer(players, &count, p2);
    
    int result = removePlayer(players, &count, "Last");
    
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_INT(1, count);
    TEST_ASSERT_EQUAL_PTR(p1, players[0]);
    
    destroyPlayer(p1);
}

void test_removePlayer_should_return_0_if_not_found(void) {
    Player* players[MAX_PLAYERS];
    int count = 0;
    
    Player* p1 = createPlayer("Only");
    addPlayer(players, &count, p1);
    
    int result = removePlayer(players, &count, "Unknown");
    
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(1, count);  // Inchangé
    
    destroyPlayer(p1);
}

// ========================================
// Tests pour sortPlayersBySpicyIndex()
// ========================================

void test_sortPlayers_should_sort_descending_by_spicyIndex(void) {
    Player* players[MAX_PLAYERS];
    int count = 0;
    
    Player* p1 = createPlayer("P1");
    p1->spicyIndex = 500;
    Player* p2 = createPlayer("P2");
    p2->spicyIndex = 1500;
    Player* p3 = createPlayer("P3");
    p3->spicyIndex = 1000;
    
    addPlayer(players, &count, p1);
    addPlayer(players, &count, p2);
    addPlayer(players, &count, p3);
    
    sortPlayersBySpicyIndex(players, count);
    
    // Ordre attendu: 1500, 1000, 500 (décroissant)
    TEST_ASSERT_EQUAL_INT(1500, players[0]->spicyIndex);
    TEST_ASSERT_EQUAL_INT(1000, players[1]->spicyIndex);
    TEST_ASSERT_EQUAL_INT(500, players[2]->spicyIndex);
    
    destroyPlayer(p1);
    destroyPlayer(p2);
    destroyPlayer(p3);
}

void test_sortPlayers_should_handle_already_sorted(void) {
    Player* players[MAX_PLAYERS];
    int count = 0;
    
    Player* p1 = createPlayer("P1");
    p1->spicyIndex = 3000;
    Player* p2 = createPlayer("P2");
    p2->spicyIndex = 2000;
    Player* p3 = createPlayer("P3");
    p3->spicyIndex = 1000;
    
    addPlayer(players, &count, p1);
    addPlayer(players, &count, p2);
    addPlayer(players, &count, p3);
    
    sortPlayersBySpicyIndex(players, count);
    
    TEST_ASSERT_EQUAL_INT(3000, players[0]->spicyIndex);
    TEST_ASSERT_EQUAL_INT(2000, players[1]->spicyIndex);
    TEST_ASSERT_EQUAL_INT(1000, players[2]->spicyIndex);
    
    destroyPlayer(p1);
    destroyPlayer(p2);
    destroyPlayer(p3);
}

void test_sortPlayers_should_handle_equal_spicyIndex(void) {
    Player* players[MAX_PLAYERS];
    int count = 0;
    
    Player* p1 = createPlayer("First");
    p1->spicyIndex = 1000;
    Player* p2 = createPlayer("Second");
    p2->spicyIndex = 1000;
    
    addPlayer(players, &count, p1);
    addPlayer(players, &count, p2);
    
    sortPlayersBySpicyIndex(players, count);
    
    // Les deux ont spicyIndex 1000, l'ordre FIFO devrait être préservé
    TEST_ASSERT_EQUAL_INT(1000, players[0]->spicyIndex);
    TEST_ASSERT_EQUAL_INT(1000, players[1]->spicyIndex);
    
    destroyPlayer(p1);
    destroyPlayer(p2);
}

// ========================================
// Tests pour displayAllPlayers()
// ========================================

void test_displayAllPlayers_should_not_crash(void) {
    Player* players[MAX_PLAYERS];
    int count = 0;
    
    Player* p1 = createPlayer("P1");
    Player* p2 = createPlayer("P2");
    
    addPlayer(players, &count, p1);
    addPlayer(players, &count, p2);
    
    displayAllPlayers(players, count);
    
    TEST_ASSERT_TRUE(1);
    
    destroyPlayer(p1);
    destroyPlayer(p2);
}

void test_displayAllPlayers_should_handle_empty_array(void) {
    Player* players[MAX_PLAYERS];
    int count = 0;
    
    displayAllPlayers(players, count);
    
    TEST_ASSERT_TRUE(1);
}

// ========================================
// Main
// ========================================

int main(void) {
    UNITY_BEGIN();
    
    printf("Module: array-utils.c\n");
    printf("=====================\n\n");
    
    // Tests addPlayer
    RUN_TEST(test_addPlayer_should_add_to_empty_array);
    RUN_TEST(test_addPlayer_should_add_multiple_players);
    RUN_TEST(test_addPlayer_should_respect_max_capacity);
    
    // Tests searchPlayerByUsername
    RUN_TEST(test_searchPlayer_should_find_existing_player);
    RUN_TEST(test_searchPlayer_should_return_minus_one_if_not_found);
    RUN_TEST(test_searchPlayer_should_return_minus_one_on_empty_array);
    RUN_TEST(test_searchPlayer_should_find_first_player);
    RUN_TEST(test_searchPlayer_should_find_last_player);
    
    // Tests removePlayer
    RUN_TEST(test_removePlayer_should_remove_from_middle);
    RUN_TEST(test_removePlayer_should_remove_first);
    RUN_TEST(test_removePlayer_should_remove_last);
    RUN_TEST(test_removePlayer_should_return_0_if_not_found);
    
    // Tests sortPlayersBySpicyIndex
    RUN_TEST(test_sortPlayers_should_sort_descending_by_spicyIndex);
    RUN_TEST(test_sortPlayers_should_handle_already_sorted);
    RUN_TEST(test_sortPlayers_should_handle_equal_spicyIndex);
    
    // Tests displayAllPlayers
    RUN_TEST(test_displayAllPlayers_should_not_crash);
    RUN_TEST(test_displayAllPlayers_should_handle_empty_array);
    
    return UNITY_END();
}

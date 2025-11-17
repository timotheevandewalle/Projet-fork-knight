/**
 * Tests unitaires - Partie 1
 * Module: player.c
 */

#include "../unity/unity.h"
#include "../../headers/player.h"

void setUp(void) {
    // Appelé avant chaque test
}

void tearDown(void) {
    // Appelé après chaque test
}

// ========================================
// Tests pour createPlayer()
// ========================================

void test_createPlayer_should_allocate_memory(void) {
    Player* p = createPlayer("TestUser");
    
    TEST_ASSERT_NOT_NULL(p);
    
    if (p != NULL) {
        destroyPlayer(p);
    }
}

void test_createPlayer_should_initialize_username(void) {
    Player* p = createPlayer("ForkMaster");
    
    TEST_ASSERT_NOT_NULL(p);
    if (p != NULL) {
        TEST_ASSERT_EQUAL_STRING("ForkMaster", p->username);
        destroyPlayer(p);
    }
}

void test_createPlayer_should_initialize_level_to_1(void) {
    Player* p = createPlayer("TestUser");
    
    TEST_ASSERT_NOT_NULL(p);
    if (p != NULL) {
        TEST_ASSERT_EQUAL_INT(1, p->level);
        destroyPlayer(p);
    }
}

void test_createPlayer_should_initialize_spicyIndex_to_1000(void) {
    Player* p = createPlayer("TestUser");
    
    TEST_ASSERT_NOT_NULL(p);
    if (p != NULL) {
        TEST_ASSERT_EQUAL_INT(1000, p->spicyIndex);
        destroyPlayer(p);
    }
}

void test_createPlayer_should_initialize_stats_to_0(void) {
    Player* p = createPlayer("TestUser");
    
    TEST_ASSERT_NOT_NULL(p);
    if (p != NULL) {
        TEST_ASSERT_EQUAL_INT(0, p->numGames);
        TEST_ASSERT_EQUAL_INT(0, p->numWins);
        TEST_ASSERT_EQUAL_INT(0, p->numLosses);
        TEST_ASSERT_EQUAL_INT(0, p->inQueue);
        destroyPlayer(p);
    }
}

void test_createPlayer_should_handle_null_username(void) {
    // Test CRITIQUE: doit gérer le cas NULL
    Player* p = createPlayer(NULL);
    
    TEST_ASSERT_NULL(p);
}

void test_createPlayer_should_handle_long_username(void) {
    // Test avec un username très long (> 30 caractères)
    const char* long_name = "ThisIsAVeryLongUsernameWithMoreThan30Characters";
    Player* p = createPlayer(long_name);
    
    TEST_ASSERT_NOT_NULL(p);
    if (p != NULL) {
        // Le nom doit être tronqué à 29 caractères (+ '\0')
        TEST_ASSERT_TRUE(strlen(p->username) < 30);
        destroyPlayer(p);
    }
}

// ========================================
// Tests pour destroyPlayer()
// ========================================

void test_destroyPlayer_should_not_crash_with_null(void) {
    // Ne doit pas planter si on passe NULL
    destroyPlayer(NULL);
    // Si on arrive ici, c'est OK
    TEST_ASSERT_TRUE(1);
}

// ========================================
// Tests pour displayPlayer()
// ========================================

void test_displayPlayer_should_not_crash(void) {
    Player* p = createPlayer("TestDisplay");
    
    TEST_ASSERT_NOT_NULL(p);
    if (p != NULL) {
        // Rediriger stdout pour capturer la sortie serait idéal,
        // mais pour l'instant on vérifie juste qu'il ne plante pas
        displayPlayer(p);
        destroyPlayer(p);
    }
    
    TEST_ASSERT_TRUE(1);
}

void test_displayPlayer_should_handle_null(void) {
    // Ne doit pas planter avec NULL
    displayPlayer(NULL);
    TEST_ASSERT_TRUE(1);
}

// ========================================
// Tests pour displayPlayerMini()
// ========================================

void test_displayPlayerMini_should_not_crash(void) {
    Player* p = createPlayer("TestMini");
    
    TEST_ASSERT_NOT_NULL(p);
    if (p != NULL) {
        displayPlayerMini(p);
        destroyPlayer(p);
    }
    
    TEST_ASSERT_TRUE(1);
}

void test_displayPlayerMini_should_handle_null(void) {
    displayPlayerMini(NULL);
    TEST_ASSERT_TRUE(1);
}

// ========================================
// Main
// ========================================

int main(void) {
    UNITY_BEGIN();
    
    printf("Module: player.c\n");
    printf("================\n\n");
    
    // Tests createPlayer
    RUN_TEST(test_createPlayer_should_allocate_memory);
    RUN_TEST(test_createPlayer_should_initialize_username);
    RUN_TEST(test_createPlayer_should_initialize_level_to_1);
    RUN_TEST(test_createPlayer_should_initialize_spicyIndex_to_1000);
    RUN_TEST(test_createPlayer_should_initialize_stats_to_0);
    RUN_TEST(test_createPlayer_should_handle_null_username);
    RUN_TEST(test_createPlayer_should_handle_long_username);
    
    // Tests destroyPlayer
    RUN_TEST(test_destroyPlayer_should_not_crash_with_null);
    
    // Tests displayPlayer
    RUN_TEST(test_displayPlayer_should_not_crash);
    RUN_TEST(test_displayPlayer_should_handle_null);
    
    // Tests displayPlayerMini
    RUN_TEST(test_displayPlayerMini_should_not_crash);
    RUN_TEST(test_displayPlayerMini_should_handle_null);
    
    return UNITY_END();
}

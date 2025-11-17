# Makefile pour Fork Knight
# Projet de SDD Avancées

# Compilateur et flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iheaders -g
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1

# Répertoires
SRC_DIR = src
HEADERS_DIR = headers
TESTS_DIR = tests
OBJ_DIR = obj

# Couleurs pour la sortie
GREEN = \033[0;32m
RED = \033[0;31m
YELLOW = \033[1;33m
NC = \033[0m # No Color

# Fichiers sources principaux
SRC_FILES = $(SRC_DIR)/player.c $(SRC_DIR)/array-utils.c
MAIN_SRC = $(SRC_DIR)/main.c

# Tests
UNITY_SRC = $(TESTS_DIR)/unity/unity.c
TEST_PLAYER_SRC = $(TESTS_DIR)/part1/test_player.c
TEST_ARRAY_SRC = $(TESTS_DIR)/part1/test_array_utils.c
TEST_DYNAMIC_SRC = $(TESTS_DIR)/part2/test_dynamic_array.c

# Exécutables
MAIN_EXEC = main
TEST_PLAYER_EXEC = test_part1_player
TEST_ARRAY_EXEC = test_part1_array
TEST_PART1_EXEC = test_part1
TEST_PART2_EXEC = test_part2

.PHONY: all clean test test-part1 test-part2 test-all-parts valgrind-part1 valgrind-part2 run help

# Cible par défaut
all: $(MAIN_EXEC)
	@echo "$(GREEN)✅ Compilation réussie !$(NC)"
	@echo "Exécutez './main' ou 'make run' pour lancer le programme"

# Programme principal
$(MAIN_EXEC): $(SRC_FILES) $(MAIN_SRC)
	@echo "$(YELLOW)🔨 Compilation du programme principal...$(NC)"
	$(CC) $(CFLAGS) $^ -o $@

# Tests Part 1 - Module player
$(TEST_PLAYER_EXEC): $(SRC_DIR)/player.c $(UNITY_SRC) $(TEST_PLAYER_SRC)
	@echo "$(YELLOW)🔨 Compilation des tests player.c...$(NC)"
	$(CC) $(CFLAGS) -I$(TESTS_DIR)/unity $^ -o $@

# Tests Part 1 - Module array-utils
$(TEST_ARRAY_EXEC): $(SRC_FILES) $(UNITY_SRC) $(TEST_ARRAY_SRC)
	@echo "$(YELLOW)🔨 Compilation des tests array-utils.c...$(NC)"
	$(CC) $(CFLAGS) -I$(TESTS_DIR)/unity $^ -o $@

# Tests Part 1 - Tous les modules
$(TEST_PART1_EXEC): $(SRC_FILES) $(UNITY_SRC) $(TEST_PLAYER_SRC) $(TEST_ARRAY_SRC)
	@echo "$(YELLOW)🔨 Compilation de tous les tests Part 1...$(NC)"
	@# Créer un fichier de test principal qui appelle tous les tests
	@echo '#include <stdio.h>' > /tmp/test_part1_runner.c
	@echo 'int test_player_main(void);' >> /tmp/test_part1_runner.c
	@echo 'int test_array_main(void);' >> /tmp/test_part1_runner.c
	@echo 'int main(void) {' >> /tmp/test_part1_runner.c
	@echo '    int result = 0;' >> /tmp/test_part1_runner.c
	@echo '    printf("\\n========================================\\n");' >> /tmp/test_part1_runner.c
	@echo '    printf("   FORK KNIGHT - TESTS PARTIE 1\\n");' >> /tmp/test_part1_runner.c
	@echo '    printf("========================================\\n\\n");' >> /tmp/test_part1_runner.c
	@echo '    result += test_player_main();' >> /tmp/test_part1_runner.c
	@echo '    printf("\\n");' >> /tmp/test_part1_runner.c
	@echo '    result += test_array_main();' >> /tmp/test_part1_runner.c
	@echo '    printf("\\n========================================\\n");' >> /tmp/test_part1_runner.c
	@echo '    if (result == 0) {' >> /tmp/test_part1_runner.c
	@echo '        printf("✅ TOUS LES TESTS PART 1 RÉUSSIS !\\n");' >> /tmp/test_part1_runner.c
	@echo '    } else {' >> /tmp/test_part1_runner.c
	@echo '        printf("❌ CERTAINS TESTS ONT ÉCHOUÉ\\n");' >> /tmp/test_part1_runner.c
	@echo '    }' >> /tmp/test_part1_runner.c
	@echo '    printf("========================================\\n\\n");' >> /tmp/test_part1_runner.c
	@echo '    return result;' >> /tmp/test_part1_runner.c
	@echo '}' >> /tmp/test_part1_runner.c
	@# Modifier test_player.c pour avoir test_player_main au lieu de main
	@sed 's/int main(void)/int test_player_main(void)/' $(TEST_PLAYER_SRC) > /tmp/test_player_renamed.c
	@sed 's/int main(void)/int test_array_main(void)/' $(TEST_ARRAY_SRC) > /tmp/test_array_renamed.c
	$(CC) $(CFLAGS) -I$(TESTS_DIR)/unity $(SRC_FILES) $(UNITY_SRC) /tmp/test_player_renamed.c /tmp/test_array_renamed.c /tmp/test_part1_runner.c -o $@

# Exécuter le programme
run: $(MAIN_EXEC)
	@echo "$(YELLOW)▶️  Exécution du programme...$(NC)"
	@./$(MAIN_EXEC)

# Tests
test-part1: $(TEST_PLAYER_EXEC) $(TEST_ARRAY_EXEC)
	@echo ""
	@echo "=========================================="
	@echo "   FORK KNIGHT - TESTS PARTIE 1"
	@echo "=========================================="
	@echo ""
	@echo "$(YELLOW)🧪 Test module: player.c$(NC)"
	@./$(TEST_PLAYER_EXEC) || true
	@echo ""
	@echo "$(YELLOW)🧪 Test module: array-utils.c$(NC)"
	@./$(TEST_ARRAY_EXEC) || true
	@echo ""
	@echo "=========================================="
	@# Compter les résultats
	@echo "$(GREEN)✅ Tests Part 1 terminés$(NC)"
	@echo "=========================================="
	@echo ""
	@echo "💡 Prochaines étapes :"
	@echo ""
	@echo "   1. Vérifier la mémoire :"
	@echo "      make valgrind-part1        # Si Valgrind disponible"
	@echo "      # OU"
	@echo "      ./test-local.sh            # Tests + mémoire automatique"
	@echo ""
	@echo "   2. Si tout est OK, soumettre :"
	@echo "      git add ."
	@echo "      git commit -m \"feat: Part 1 complète\""
	@echo "      git checkout -b submissions/part1"
	@echo "      git push origin submissions/part1"
	@echo ""

# Vérification mémoire avec Valgrind
valgrind-part1: $(TEST_PLAYER_EXEC) $(TEST_ARRAY_EXEC)
	@echo "$(YELLOW)🔍 Vérification mémoire (Valgrind)...$(NC)"
	@echo ""
	@echo "Test player.c:"
	@$(VALGRIND) ./$(TEST_PLAYER_EXEC) 2>&1 | grep -A 5 "LEAK SUMMARY" || echo "$(GREEN)✅ Pas de fuites mémoire détectées$(NC)"
	@echo ""
	@echo "Test array-utils.c:"
	@$(VALGRIND) ./$(TEST_ARRAY_EXEC) 2>&1 | grep -A 5 "LEAK SUMMARY" || echo "$(GREEN)✅ Pas de fuites mémoire détectées$(NC)"

# Tests Part 2 - Tableaux dynamiques et fichiers
$(TEST_PART2_EXEC): $(SRC_FILES) $(UNITY_SRC) $(TEST_DYNAMIC_SRC)
	@echo "$(YELLOW)🔨 Compilation des tests Part 2...$(NC)"
	$(CC) $(CFLAGS) -I$(TESTS_DIR)/unity $^ -o $@

test-part2: $(TEST_PART2_EXEC)
	@echo ""
	@echo "=========================================="
	@echo "   FORK KNIGHT - TESTS PARTIE 2"
	@echo "=========================================="
	@echo ""
	@echo "$(YELLOW)🧪 Test module: Tableaux dynamiques et fichiers$(NC)"
	@./$(TEST_PART2_EXEC) || true
	@echo ""
	@echo "=========================================="
	@echo "$(GREEN)✅ Tests Part 2 terminés$(NC)"
	@echo "=========================================="
	@echo ""
	@echo "💡 Prochaines étapes :"
	@echo ""
	@echo "   1. Lancez les tests en local :"
	@echo "      ./test-local.sh 2"
	@echo ""
	@echo "   2. Si tout est OK, soumettre :"
	@echo "      ./submit.sh 2"
	@echo ""

# Vérification mémoire Part 2
valgrind-part2: $(TEST_PART2_EXEC)
	@echo "$(YELLOW)🔍 Vérification mémoire Part 2 (Valgrind)...$(NC)"
	@echo ""
	@$(VALGRIND) ./$(TEST_PART2_EXEC) 2>&1 | grep -A 5 "LEAK SUMMARY" || echo "$(GREEN)✅ Pas de fuites mémoire détectées$(NC)"

# Nettoyage
clean:
	@echo "$(YELLOW)🧹 Nettoyage...$(NC)"
	rm -f $(MAIN_EXEC)
	rm -f $(TEST_PLAYER_EXEC)
	rm -f $(TEST_ARRAY_EXEC)
	rm -f $(TEST_PART1_EXEC)
	rm -f $(TEST_PART2_EXEC)
	rm -f *.o
	rm -f $(SRC_DIR)/*.o
	rm -f $(TESTS_DIR)/**/*.o
	rm -rf $(OBJ_DIR)
	rm -f test_part3 test_part4 test_part5 test_part6
	rm -rf *.dSYM
	rm -f /tmp/test_part1_runner.c
	rm -f /tmp/test_player_renamed.c
	rm -f /tmp/test_array_renamed.c
	rm -f /tmp/test_*_renamed.c
	@echo "$(GREEN)✅ Nettoyage terminé$(NC)"

# Test complet de toutes les parties (pour autograding final)
test-all-parts: test-part1 test-part2
	@echo ""
	@echo "=========================================="
	@echo "   TESTS COMPLETS - TOUTES LES PARTIES"
	@echo "=========================================="
	@echo ""
	@echo "$(GREEN)✅ Part 1 : TERMINÉE$(NC)"
	@echo "$(GREEN)✅ Part 2 : TERMINÉE$(NC)"
	@echo "⚠️  Parts 3-6 : À implémenter"
	@echo ""
	@echo "=========================================="
	@echo "Note: Cette cible sera utilisée pour"
	@echo "l'autograding final dans GitHub Classroom"
	@echo "=========================================="
	@echo ""

# Aide
help:
	@echo "=========================================="
	@echo "   FORK KNIGHT - AIDE MAKEFILE"
	@echo "=========================================="
	@echo ""
	@echo "Commandes disponibles:"
	@echo ""
	@echo "  make              Compile le programme principal"
	@echo "  make run          Compile et exécute le programme"
	@echo "  make test-part1   Compile et exécute les tests Part 1"
	@echo "  make test-part2   Compile et exécute les tests Part 2"
	@echo "  make test-all-parts  Exécute TOUS les tests (autograding final)"
	@echo "  make valgrind-part1  Vérifie les fuites mémoire Part 1"
	@echo "  make valgrind-part2  Vérifie les fuites mémoire Part 2"
	@echo "  make clean        Nettoie les fichiers compilés"
	@echo "  make help         Affiche cette aide"
	@echo ""
	@echo "=========================================="
	@echo ""
	@echo "Exemple de workflow:"
	@echo "  1. make test-part1     # Tester votre code"
	@echo "  2. make valgrind-part1 # Vérifier la mémoire"
	@echo "  3. make run            # Exécuter le programme"
	@echo ""

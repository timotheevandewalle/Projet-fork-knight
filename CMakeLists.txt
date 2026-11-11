# CMakeLists.txt pour Fork Knight
# Compatible avec CLion et autres IDEs modernes
cmake_minimum_required(VERSION 3.15)
project(ForkKnight C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -g")

# Inclure les headers
include_directories(headers)
include_directories(tests/unity)

# ==========================================
# Programme Principal
# ==========================================
set(MAIN_SOURCES
    src/player.c
    src/array-utils.c
    src/main.c
        src/list-utils.c
        headers/list-utils.h
        headers/priority-queue.h
        src/priority-queue.c
        headers/match.h
        src/match.c
)

add_executable(main ${MAIN_SOURCES})

# ==========================================
# Tests Partie 1
# ==========================================
set(COMMON_SOURCES
    src/player.c
    src/array-utils.c
    tests/unity/unity.c
)

# Tests module player
add_executable(test_part1_player
    ${COMMON_SOURCES}
    tests/part1/test_player.c
)

# Tests module array-utils
add_executable(test_part1_array
    ${COMMON_SOURCES}
    tests/part1/test_array_utils.c
)

# ==========================================
# Configuration CLion
# ==========================================
# Définir les configurations de run pour CLion
# Les étudiants pourront sélectionner :
# - "main" : Exécuter le programme
# - "test_part1_player" : Tests du module player
# - "test_part1_array" : Tests du module array-utils

# ==========================================
# Commandes personnalisées (optionnel)
# ==========================================
# Valgrind (si disponible)
find_program(VALGRIND valgrind)
if(VALGRIND)
    add_custom_target(valgrind-part1-player
        COMMAND ${VALGRIND} --leak-check=full --error-exitcode=1 ./test_part1_player
        DEPENDS test_part1_player
        COMMENT "Vérification mémoire avec Valgrind (player.c)"
    )
    
    add_custom_target(valgrind-part1-array
        COMMAND ${VALGRIND} --leak-check=full --error-exitcode=1 ./test_part1_array
        DEPENDS test_part1_array
        COMMENT "Vérification mémoire avec Valgrind (array-utils.c)"
    )
endif()

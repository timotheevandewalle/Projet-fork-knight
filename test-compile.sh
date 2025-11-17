#!/bin/bash

# Script de test de compilation simple
# Compile et exécute votre sandbox (main.c) pour tester manuellement vos fonctions

set -e

echo "🔨 Test de compilation (sandbox main.c)"
echo "==============================================="
echo ""

# Vérifier que GCC ou Clang est disponible
if command -v gcc &> /dev/null; then
    CC=gcc
elif command -v clang &> /dev/null; then
    CC=clang
else
    echo "❌ Erreur: Aucun compilateur trouvé (gcc ou clang)"
    echo "   Installez GCC ou Clang pour continuer"
    exit 1
fi

echo "Compilateur : $CC"
echo ""

# Nettoyer
rm -f main 2>/dev/null || true

echo "Compilation de main.c (sandbox)..."
$CC -Wall -Wextra -Iheaders -g \
    src/player.c \
    src/array-utils.c \
    src/main.c \
    -o main

if [ $? -eq 0 ]; then
    echo "✅ Compilation réussie !"
    echo ""
    echo "==============================================="
    echo "Exécution du programme :"
    echo "==============================================="
    echo ""
    
    # Exécuter
    ./main
    
    EXIT_CODE=$?
    
    echo ""
    echo "==============================================="
    
    if [ $EXIT_CODE -eq 0 ]; then
        echo "✅ Programme terminé avec succès"
        echo ""
        echo "💡 Prochaine étape : Validez avec les tests complets"
        echo ""
        echo "   ./test-local.sh <partie>    # Tests unitaires + vérification mémoire"
        echo ""
    else
        echo "❌ Programme terminé avec erreur (code $EXIT_CODE)"
        echo ""
        echo "💡 Vérifiez votre code et relancez ./test-compile.sh"
    fi
    
    exit $EXIT_CODE
else
    echo "❌ Erreurs de compilation !"
    echo ""
    echo "💡 Lisez les messages d'erreur ci-dessus"
    echo "   Corrigez votre code et relancez ./test-compile.sh"
    exit 1
fi

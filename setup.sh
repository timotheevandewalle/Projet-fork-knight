#!/bin/bash

# Script de configuration initiale pour Fork Knight
# Ce script aide à configurer l'environnement de développement

echo "🍴⚔️  Fork Knight - Configuration Initiale"
echo "=========================================="
echo ""

# Détection de l'OS
OS="$(uname -s)"
case "${OS}" in
    Linux*)     MACHINE=Linux;;
    Darwin*)    MACHINE=Mac;;
    CYGWIN*|MINGW*|MSYS*)  MACHINE=Windows;;
    *)          MACHINE="UNKNOWN:${OS}"
esac

echo "🖥️  Système détecté : $MACHINE"
echo ""

# Vérifier Git
echo "📦 Vérification des dépendances..."
if command -v git &> /dev/null; then
    echo "  ✅ Git installé ($(git --version))"
else
    echo "  ❌ Git non trouvé - Installer Git : https://git-scm.com/"
    exit 1
fi

# Vérifier GCC/Clang
if command -v gcc &> /dev/null; then
    echo "  ✅ GCC installé ($(gcc --version | head -n 1))"
    COMPILER="gcc"
elif command -v clang &> /dev/null; then
    echo "  ✅ Clang installé ($(clang --version | head -n 1))"
    COMPILER="clang"
else
    echo "  ❌ Aucun compilateur C trouvé"
    echo "     Installer GCC ou Clang selon votre système"
    exit 1
fi

# Vérifier CMake (optionnel mais recommandé)
if command -v cmake &> /dev/null; then
    echo "  ✅ CMake installé ($(cmake --version | head -n 1))"
    HAS_CMAKE=true
else
    echo "  ⚠️  CMake non trouvé (optionnel mais recommandé pour CLion)"
    echo "     Installation : "
    if [ "$MACHINE" = "Mac" ]; then
        echo "       brew install cmake"
    elif [ "$MACHINE" = "Linux" ]; then
        echo "       sudo apt install cmake  # Ubuntu/Debian"
        echo "       sudo dnf install cmake  # Fedora"
    fi
    HAS_CMAKE=false
fi

# Vérifier Make
if command -v make &> /dev/null; then
    echo "  ✅ Make installé ($(make --version | head -n 1))"
    HAS_MAKE=true
else
    echo "  ⚠️  Make non trouvé (optionnel)"
    HAS_MAKE=false
fi

    # Vérifier Valgrind (seulement sur Linux)
    if [ "$MACHINE" = "Linux" ]; then
        if command -v valgrind &> /dev/null; then
            echo "  ✅ Valgrind installé ($(valgrind --version))"
        else
            echo "  ⚠️  Valgrind non trouvé (recommandé pour la vérification mémoire)"
            echo "     Installation : "
            echo "       sudo apt install valgrind  # Ubuntu/Debian"
            echo "       sudo dnf install valgrind  # Fedora"
        fi
    elif [ "$MACHINE" = "Mac" ]; then
        echo "  ℹ️  Valgrind non disponible sur macOS"
        echo "     → Utilisation d'AddressSanitizer à la place"
    elif [ "$MACHINE" = "Windows" ]; then
        echo "  ℹ️  Valgrind non disponible sur Windows"
        echo "     → Tests mémoire disponibles sur GitHub Actions (Linux)"
    fi

echo ""
echo "=========================================="
echo ""

# Créer les dossiers nécessaires
echo "📁 Création des dossiers de sortie..."
mkdir -p output
touch output/.gitkeep
mkdir -p input
touch input/.gitkeep
echo "  ✅ Dossiers créés"
echo ""

# Configuration Git
echo "🔧 Configuration Git recommandée..."
git config --local core.autocrlf input  # Normaliser les fins de ligne
echo "  ✅ Configuration appliquée"
echo ""

# Afficher les prochaines étapes
echo "=========================================="
echo "🎉 Configuration terminée !"
echo "=========================================="
echo ""
echo "📚 Prochaines étapes :"
echo ""
echo "  1️⃣  Lire les spécifications du projet sur la page web du cours"
echo ""
echo "  2️⃣  Commencer à coder pour la Partie 1 :"
echo "       - Éditer src/player.c"
echo "       - Éditer src/array-utils.c"
echo ""
echo "  3️⃣  Tester votre code :"
echo "       - ./test-compile.sh     # Exécute votre main et vérifie la compilation"
echo "       - ./test-local.sh 1     # Tests unitaires + mémoire (obligatoire avant soumission) pour la Partie 1"
echo ""
echo "  4️⃣  Soumettre :"
echo "       - ./submit.sh 1         # Soumettre la Partie 1"   
echo ""
echo "=========================================="
echo "Bon courage !"
echo "=========================================="
echo ""

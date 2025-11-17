#!/bin/bash

# Script de soumission pour Fork Knight
# Automatise la création de la branche de soumission

set -e

# Vérifier l'argument
if [ $# -eq 0 ]; then
    echo "❌ Erreur : Numéro de partie manquant"
    echo ""
    echo "Usage: $0 <numéro_partie>"
    echo ""
    echo "Exemples:"
    echo "  $0 1     # Soumet la partie 1"
    echo "  $0 2     # Soumet la partie 2"
    echo ""
    exit 1
fi

PART=$1
BRANCH_NAME="submissions/part${PART}"

echo "🚀 Soumission Fork Knight - Partie $PART"
echo "=========================================="
echo ""

# Vérifier que nous sommes dans un repo git
if [ ! -d .git ]; then
    echo "❌ Erreur : Ce n'est pas un dépôt Git"
    echo "   Exécutez ce script depuis la racine de votre projet"
    exit 1
fi

# Vérifier qu'on est sur main
CURRENT_BRANCH=$(git branch --show-current)
if [ "$CURRENT_BRANCH" != "main" ]; then
    echo "⚠️  Vous n'êtes pas sur la branche 'main' (actuellement sur '$CURRENT_BRANCH')"
    echo ""
    read -p "Voulez-vous retourner sur 'main' ? (o/N) " -n 1 -r
    echo ""
    if [[ $REPLY =~ ^[Oo]$ ]]; then
        git checkout main
    else
        echo "❌ Soumission annulée"
        exit 1
    fi
fi

# Vérifier qu'il n'y a pas de modifications non commitées
if ! git diff-index --quiet HEAD --; then
    echo "⚠️  Vous avez des modifications non commitées"
    echo ""
    git status --short
    echo ""
    read -p "Voulez-vous commiter ces modifications maintenant ? (o/N) " -n 1 -r
    echo ""
    if [[ $REPLY =~ ^[Oo]$ ]]; then
        git add .
        read -p "Message de commit : " COMMIT_MSG
        if [ -z "$COMMIT_MSG" ]; then
            COMMIT_MSG="feat: Implémentation Partie $PART"
        fi
        git commit -m "$COMMIT_MSG"
        echo "✅ Modifications commitées"
        echo ""
        echo "📤 Push sur main..."
        git push origin main
        echo "✅ Modifications poussées sur main"
    else
        echo ""
        echo "❌ Commitez vos modifications avant de soumettre :"
        echo ""
        echo "   git add ."
        echo "   git commit -m \"feat: Part $PART complète\""
        echo "   git push origin main"
        echo "   ./submit.sh $PART"
        echo ""
        exit 1
    fi
fi

# Vérifier que main est à jour avec origin
echo ""
echo "🔍 Vérification que main est à jour..."
git fetch origin main
LOCAL=$(git rev-parse main)
REMOTE=$(git rev-parse origin/main)

if [ "$LOCAL" != "$REMOTE" ]; then
    echo "⚠️  Votre branche main locale n'est pas à jour avec origin/main"
    echo ""
    read -p "Voulez-vous push vos commits sur main maintenant ? (o/N) " -n 1 -r
    echo ""
    if [[ $REPLY =~ ^[Oo]$ ]]; then
        git push origin main
        echo "✅ main poussé vers origin"
    else
        echo "❌ Soumission annulée"
        echo "   Poussez vos commits sur main avant de soumettre :"
        echo "   git push origin main"
        exit 1
    fi
fi

# Vérifier si la branche existe déjà localement
if git show-ref --verify --quiet refs/heads/$BRANCH_NAME; then
    echo "⚠️  La branche $BRANCH_NAME existe déjà localement"
    echo ""
    read -p "Voulez-vous la supprimer et la recréer ? (o/N) " -n 1 -r
    echo ""
    if [[ $REPLY =~ ^[Oo]$ ]]; then
        git branch -D $BRANCH_NAME
        echo "✅ Branche locale supprimée"
    else
        echo "❌ Soumission annulée"
        exit 1
    fi
fi

# Vérifier si la branche existe sur le remote
if git ls-remote --heads origin $BRANCH_NAME | grep -q $BRANCH_NAME; then
    echo "⚠️  La branche $BRANCH_NAME existe déjà sur GitHub"
    echo ""
    echo "Cela signifie que vous avez déjà soumis cette partie."
    echo "Voulez-vous resoummettre (écrasera l'ancienne soumission) ?"
    echo ""
    read -p "Resoummettre ? (o/N) " -n 1 -r
    echo ""
    if [[ $REPLY =~ ^[Oo]$ ]]; then
        echo "🗑️  Suppression de l'ancienne soumission..."
        git push origin --delete $BRANCH_NAME
        echo "✅ Ancienne soumission supprimée"
    else
        echo "❌ Soumission annulée"
        exit 1
    fi
fi

echo ""
echo "=========================================="
echo "📋 RÉCAPITULATIF"
echo "=========================================="
echo ""
echo "Branche actuelle : main"
echo "Dernier commit   : $(git log -1 --pretty=format:'%h - %s')"
echo "Branche cible    : $BRANCH_NAME"
echo ""
echo "=========================================="
echo ""

read -p "Confirmer la soumission ? (o/N) " -n 1 -r
echo ""

if [[ ! $REPLY =~ ^[Oo]$ ]]; then
    echo "❌ Soumission annulée"
    exit 1
fi

echo ""
echo "🚀 Création et envoi de la soumission..."
echo ""

# Créer la branche de soumission
git checkout -b $BRANCH_NAME

# Pousser vers GitHub
git push origin $BRANCH_NAME

# Retourner sur main
git checkout main

echo ""
echo "=========================================="
echo "✅ SOUMISSION RÉUSSIE !"
echo "=========================================="
echo ""
echo "La partie $PART a été soumise avec succès."
echo ""
echo "⚠️  IMPORTANT :"
echo "   • Ne créez PAS de Pull Request vers 'main' !"
echo "   • La branche $BRANCH_NAME est un snapshot immutable"
echo "   • Elle ne doit jamais être mergée vers main"
echo ""
echo "📍 Prochaines étapes :"
echo ""
echo "1. Allez sur GitHub → Onglet 'Actions'"
echo "2. Vous verrez le workflow 'Validate Part $PART Submission' en cours"
echo "3. Attendez les résultats (2-3 minutes)"
echo "4. Consultez le rapport détaillé dans les commentaires"
echo ""
echo "🔗 URL : https://github.com/$(git config --get remote.origin.url | sed 's/.*github.com[:/]\(.*\)\.git/\1/')/actions"
echo ""
echo "💡 Conseil : Vous pouvez continuer à travailler sur 'main'"
echo "   pendant que les tests s'exécutent."
echo ""

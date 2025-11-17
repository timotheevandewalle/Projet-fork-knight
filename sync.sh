#!/bin/bash

# Script de synchronisation pour Fork Knight
# Récupère les modifications de votre binôme sans sauvegarder vos changements

set +e  # Ne pas quitter en cas d'erreur

echo "🔄 Synchronisation Fork Knight"
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
        echo "❌ Synchronisation annulée"
        exit 1
    fi
fi

# Vérifier s'il y a des modifications locales
HAS_LOCAL_CHANGES=false
if ! git diff-index --quiet HEAD -- 2>/dev/null; then
    HAS_LOCAL_CHANGES=true
fi

# Récupérer les informations du remote
echo "📡 Vérification des modifications sur GitHub..."
git fetch origin main 2>/dev/null

# Vérifier si le remote a des changements
LOCAL=$(git rev-parse main 2>/dev/null)
REMOTE=$(git rev-parse origin/main 2>/dev/null)

if [ "$LOCAL" = "$REMOTE" ]; then
    echo ""
    echo "✅ Vous êtes déjà à jour avec votre binôme !"
    echo ""
    if [ "$HAS_LOCAL_CHANGES" = true ]; then
        echo "💡 Vous avez des modifications locales non sauvegardées."
        echo "   Utilisez ./save.sh pour les sauvegarder sur GitHub."
    fi
    exit 0
fi

echo ""
echo "📥 Votre binôme a fait des modifications. Récupération..."
echo ""

# Si on a des modifications locales, on les met de côté temporairement
if [ "$HAS_LOCAL_CHANGES" = true ]; then
    echo "⚠️  Vous avez des modifications locales non sauvegardées."
    echo ""
    read -p "Voulez-vous les mettre de côté temporairement ? (o/N) " -n 1 -r
    echo ""
    
    if [[ ! $REPLY =~ ^[Oo]$ ]]; then
        echo ""
        echo "❌ Synchronisation annulée"
        echo ""
        echo "💡 Options :"
        echo "   1. Sauvegardez vos modifications : ./save.sh"
        echo "   2. Ou acceptez de les mettre de côté temporairement"
        echo ""
        exit 1
    fi
    
    echo "💾 Sauvegarde temporaire de vos modifications..."
    git stash push -m "Auto-stash during sync" >/dev/null 2>&1
    STASHED=true
else
    STASHED=false
fi

# Récupérer les changements
git pull origin main --no-edit 2>&1 | tee /tmp/git_pull_output.txt
PULL_EXIT_CODE=${PIPESTATUS[0]}

# Restaurer les modifications locales si elles ont été mises de côté
if [ "$STASHED" = true ]; then
    echo ""
    echo "♻️  Restauration de vos modifications..."
    git stash pop >/dev/null 2>&1
    STASH_EXIT_CODE=$?
    
    if [ $STASH_EXIT_CODE -ne 0 ]; then
        echo ""
        echo "⚠️  ═══════════════════════════════════════════════"
        echo "⚠️  CONFLIT DÉTECTÉ !"
        echo "⚠️  ═══════════════════════════════════════════════"
        echo ""
        echo "Vous et votre binôme avez modifié les mêmes fichiers."
        echo ""
        echo "📋 Fichiers en conflit :"
        git diff --name-only --diff-filter=U | sed 's/^/   • /'
        echo ""
        echo "🔧 Comment résoudre ?"
        echo ""
        echo "1. Ouvrez les fichiers listés ci-dessus"
        echo "2. Cherchez les marqueurs de conflit :"
        echo "   <<<<<<< HEAD"
        echo "   (code de votre binôme)"
        echo "   ======="
        echo "   (votre code)"
        echo "   >>>>>>> "
        echo ""
        echo "3. Choisissez quelle version garder (ou fusionnez)"
        echo "4. Supprimez les marqueurs <<<, ===, >>>"
        echo "5. Sauvegardez : ./save.sh"
        echo ""
        echo "💡 Astuce : Discutez avec votre binôme pour décider ensemble"
        echo ""
        exit 1
    fi
fi

if [ $PULL_EXIT_CODE -eq 0 ]; then
    echo ""
    echo "✅ Synchronisation réussie !"
    echo ""
    
    # Afficher un résumé des changements
    COMMITS_BEHIND=$(git rev-list --count HEAD..origin/main 2>/dev/null || echo "0")
    if [ "$COMMITS_BEHIND" != "0" ]; then
        echo "📊 Changements récupérés :"
        git log HEAD..origin/main --oneline --no-decorate 2>/dev/null | sed 's/^/   • /'
    fi
    
    echo ""
    if [ "$HAS_LOCAL_CHANGES" = true ]; then
        echo "💡 Vos modifications locales ont été préservées."
        echo "   N'oubliez pas de les sauvegarder : ./save.sh"
    else
        echo "💡 Vous pouvez maintenant continuer à travailler."
    fi
    echo ""
else
    # Vérifier s'il y a des conflits de merge
    if git diff --name-only --diff-filter=U | grep -q .; then
        echo ""
        echo "⚠️  ═══════════════════════════════════════════════"
        echo "⚠️  CONFLIT DÉTECTÉ !"
        echo "⚠️  ═══════════════════════════════════════════════"
        echo ""
        echo "Vous et votre binôme avez modifié les mêmes fichiers."
        echo ""
        echo "📋 Fichiers en conflit :"
        git diff --name-only --diff-filter=U | sed 's/^/   • /'
        echo ""
        echo "🔧 Comment résoudre ?"
        echo ""
        echo "1. Ouvrez les fichiers listés ci-dessus"
        echo "2. Cherchez les marqueurs de conflit :"
        echo "   <<<<<<< HEAD"
        echo "   (code de votre binôme)"
        echo "   ======="
        echo "   (votre code)"
        echo "   >>>>>>> "
        echo ""
        echo "3. Choisissez quelle version garder (ou fusionnez)"
        echo "4. Supprimez les marqueurs <<<, ===, >>>"
        echo "5. Sauvegardez : ./save.sh"
        echo ""
        echo "💡 Astuce : Discutez avec votre binôme pour décider ensemble"
        echo ""
        exit 1
    else
        echo ""
        echo "❌ Erreur lors de la synchronisation"
        echo "   Consultez l'enseignant si le problème persiste"
        echo ""
        exit 1
    fi
fi

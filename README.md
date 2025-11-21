[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/ofFaAvIC)
# Fork Knight - Système de Matchmaking
> Projet de synthèse - SDD Avancées - XTI102

## 🚀 Démarrage rapide

```bash
# 1️⃣ Vérifier les prérequis (une seule fois au démarrage du projet)
./setup.sh

# 2️⃣ Tester rapidement votre code dans le src/main.c
./test-compile.sh

# 3️⃣ Sauvegarder votre travail (sur GitHub)
./save.sh

# 4️⃣ Lancer les tests complets avant soumission pour la partie X
./test-local.sh X

# 5️⃣ Soumettre quand tout est OK
./submit.sh X
```

## 🧪 Sandbox de Test (main.c)

Le fichier `src/main.c` est un **sandbox personnel** pour tester vos fonctions manuellement :

```bash
./test-compile.sh
```

**⚠️ Important :**
- Ce fichier **n'est PAS** utilisé par les tests automatiques
- Vous pouvez le modifier **librement** pour vos tests personnels


## 📜 Scripts Disponibles

| Script | Description |
|--------|-------------|
| `./setup.sh` | Vérifie que tous les outils nécessaires sont installés |
| `./sync.sh` | 🆕 Récupère les modifications de votre binôme |
| `./test-compile.sh` | Compile et exécute le sandbox (développement rapide) |
| `./save.sh [message]` | Sauvegarde votre travail sur GitHub (avec sync auto) |
| `./test-local.sh X` | Tests complets + vérification mémoire (avant soumission) |
| `./submit.sh X` | Soumet officiellement la partie X pour évaluation |


### 🪟 Spécificités Windows

Si vous travaillez sur **Windows** (Git Bash/MinGW), notez que :

- ✅ **Compilation** : Fonctionne normalement avec GCC
- ✅ **Tests** : Tous les tests Unity fonctionnent
- ⚠️ **Vérification mémoire locale** : Non disponible
  - Valgrind n'existe pas sur Windows
  - AddressSanitizer n'est pas supporté par MinGW
  - Le script `./test-local.sh` compile et exécute les tests, mais **ne peut pas** vérifier la mémoire localement

**💡 Solution :** GitHub Actions utilisera **Valgrind sur Linux** lors de la soumission pour vérifier automatiquement les fuites mémoire.

**⚠️ Important :** Assurez-vous de bien libérer toute mémoire allouée avec `malloc()/calloc()` en utilisant `free()` !

## 👥 Travail en Binôme

Ce projet se fait **en binôme**. Les deux étudiants travaillent sur le **même dépôt GitHub**.

### Workflow recommandé :

```bash
# === Début de session ===
./sync.sh        # Récupérer le travail de votre binôme

# === Développement ===
# travail sur une fonction
./test-compile.sh
./save.sh        # Synchronisation automatique + sauvegarde

# === Fin de session ===
./save.sh        # Votre binôme pourra récupérer votre travail
```

### 💡 Conseils pour bien collaborer :

1. **Communiquez** : Décidez qui fait quoi avant de commencer
2. **Synchronisez souvent** : Utilisez `./sync.sh` en début de session
3. **Sauvegardez régulièrement** : `./save.sh` synchronise et sauvegarde automatiquement
4. **Évitez les conflits** : Ne travaillez pas sur les mêmes fonctions en même temps

### 🚨 En cas de conflit :

Si vous et votre binôme modifiez le même fichier simultanément, le script affichera un message clair :

```
⚠️  CONFLIT DÉTECTÉ !

📋 Fichiers en conflit :
   • src/player.c

🔧 Comment résoudre ?
1. Ouvrez le fichier
2. Cherchez les marqueurs <<<<<<, ======, >>>>>>
3. Choisissez quelle version garder
4. Supprimez les marqueurs
5. Relancez : ./save.sh
```

**💬 Astuce** : Discutez avec votre binôme pour décider ensemble quelle version garder !

## � Workflow Typique

```bash
## 🔄 Workflow Typique

### 👤 Travail Solo (pendant une session)

# === Début de session ===
./sync.sh        # Récupérer le travail de votre binôme

# === Développement ===
# 1. Coder une fonction
vim src/player.c

# 2. Tester rapidement
./test-compile.sh

# 3. Sauvegarder (synchronisation automatique)
./save.sh Implémentation createPlayer

# 4. Répéter pour chaque fonction...

# === Fin de session ===
./save.sh        # Votre binôme pourra récupérer votre travail
```

### 👥 Travail en Binôme (en même temps)

```bash
# Étudiant A travaille sur player.c et a fini
./save.sh Implémentation createPlayer

# Étudiant B travaille sur array-utils.c et a fini
./sync.sh        # Récupère le travail de A
./save.sh Implémentation addPlayer

# Étudiant A continue
./sync.sh        # Récupère le travail de B
./save.sh Implémentation destroyPlayer
```

## ⚠️ Règles Importantes

### Pour tous :
- ✅ Sauvegardez souvent avec `./save.sh` (au moins 1x par session)
- ✅ Testez avec `./test-local.sh` avant de soumettre
- ❌ Ne modifiez jamais les branches `submissions/*`
- ❌ Pour resoummettre, utilisez simplement `./submit.sh X` à nouveau

### Pour le travail en binôme :
- ✅ Commencez toujours par `./sync.sh` en début de session
- ✅ Communiquez : "Je travaille sur player.c, toi sur array-utils.c"
- ✅ Sauvegardez régulièrement : `./save.sh` synchronise automatiquement
- ❌ Évitez de modifier le même fichier simultanément

## �📚 Documentation Complète

Toutes les informations concernant le projet **Fork Knight** sont disponibles sur la page web du cours : [https://cs-efrei.github.io/xti102-sdda/content/projet/fiche-projet.html](https://cs-efrei.github.io/xti102-sdda/content/projet/fiche-projet.html)
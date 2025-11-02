# Laying Grass Game - C++ Implementation

Jeu de placement de tuiles inspiré de "The Devil's Plan" (Episode 8)

## 🎮 Règles du Jeu

- **Joueurs** : 2 à 9 joueurs
- **Grille** : 20×20 (2-4 joueurs) ou 30×30 (5-9 joueurs)
- **Rounds** : 9 rounds par joueur
- **Tuiles** : 96 types différents (rotations et flips possibles)
- **Objectif** : Créer le plus grand territoire carré

### Mécaniques Spéciales

- **Coupons d'échange** : Permet de choisir parmi les 5 prochaines tuiles
- **Cases Bonus** :
  - **Tile Exchange** : Gagne un coupon d'échange
  - **Stone** : Place une pierre (bloque les adversaires)
  - **Robbery** : Vole une tuile à un adversaire

## 📁 Structure du Projet

```
laying_grass_cpp/
├── include/          # En-têtes (.hpp)
├── src/              # Sources (.cpp)
├── docs/             # Documentation
├── tests/            # Tests unitaires
├── CMakeLists.txt    # Build CMake
└── Makefile          # Build Make
```

## 🚀 Compilation

### Avec CMake
```bash
mkdir build
cd build
cmake ..
make
```

### Avec Makefile
```bash
make
```

## ▶️ Exécution

```bash
./laying_grass
# ou
./build/laying_grass
```

## 📝 Modules Principaux

- **Game** : Logique principale du jeu
- **Board** : Gestion de la grille et territoires
- **Tile** : Représentation et transformations des tuiles
- **Queue** : File d'attente des tuiles
- **Player** : État et ressources des joueurs
- **Validator** : Validation des placements
- **Algorithms** : Algorithmes (plus grand carré, connectivité)
- **Bonus** : Gestion des cases bonus

## 📋 Dépendances

- C++17 ou supérieur
- Compilateur supportant C++17 (GCC, Clang, MSVC)

## 📚 Documentation

Voir `docs/ARCHITECTURE.md` pour l'architecture détaillée.

## 🎯 Fonctionnalités

- [x] Architecture modulaire
- [ ] Implémentation des tuiles (96 types)
- [ ] Système de queue avec échange
- [ ] Validation des placements
- [ ] Détection du plus grand carré
- [ ] Cases bonus
- [ ] Interface CLI
- [ ] Tests unitaires

---

**Date limite** : 13 novembre 2025


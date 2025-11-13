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

- [docs/USER_DOCUMENTATION.md](docs/USER_DOCUMENTATION.md) : prise en main et déroulement d’une partie via la CLI.
- [docs/TECHNICAL_DOCUMENTATION.md](docs/TECHNICAL_DOCUMENTATION.md) : architecture, décisions d’implémentation et détails techniques.
- [docs/DESIGN.md](docs/DESIGN.md) : règles détaillées, cycle complet et bonus.
- [docs/STRUCTURE.md](docs/STRUCTURE.md) : arborescence, dépendances entre modules et bonnes pratiques.

## 🎯 Fonctionnalités

- [x] Architecture modulaire
- [x] Implémentation des tuiles (96 types)
- [x] Système de queue avec échange
- [x] Validation des placements
- [x] Détection du plus grand carré
- [x] Cases bonus
- [x] Interface CLI

---



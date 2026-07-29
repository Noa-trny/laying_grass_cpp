# Laying Grass Game - C++ Implementation

Jeu de placement de tuiles inspiré de "The Devil's Plan" (Episode 8)

Lien vers le repo : https://github.com/Noa-trny/laying_grass_cpp

**C++17 pur, zéro dépendance** — bibliothèque standard uniquement, jusqu'au parseur JSON
de `data/tiles.json` écrit à la main (`TileParser`).

## 🎮 Règles du Jeu

- **Joueurs** : 2 à 9 joueurs
- **Grille** : 20×20 (2-4 joueurs) ou 30×30 (5-9 joueurs)
- **Rounds** : 9 rounds par joueur
- **Tuiles** : 96 types différents (rotations et flips possibles)
- **Objectif** : posséder le plus grand carré plein en fin de partie — à défaut, le plus grand territoire total départage

### Mécaniques Spéciales

- **Coupons d'échange** : Permet de choisir parmi les 5 prochaines tuiles
- **Cases Bonus** :
  - **Tile Exchange** : Gagne un coupon d'échange
  - **Stone** : Place une pierre (bloque les adversaires)
  - **Robbery** : annexe une case du territoire d'un adversaire

## 📁 Structure du Projet

```
laying_grass_cpp/
├── include/          # En-têtes (.hpp)
├── src/              # Sources (.cpp)
├── docs/             # Documentation
├── data/tiles.json   # Les 96 formes de tuiles
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

## 🧠 Ce que ce projet montre

RAII, `enum class`, `constexpr`, STL — et une séparation nette entre règles (`Game`),
validation (`Validator`), algorithmes (`Algorithms::findLargestSquare`, connexité) et
affichage console. Le choix d'écrire le parseur plutôt que d'importer une bibliothèque
pour un seul fichier résume l'esprit du projet.

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



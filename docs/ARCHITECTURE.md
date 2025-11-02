# Architecture du Projet Laying Grass

## Vue d'ensemble

Ce projet implémente un jeu de placement de tuiles inspiré de "The Devil's Plan" (Episode 8) en C++.

## Structure des Répertoires

```
laying_grass_cpp/
├── include/          # Fichiers d'en-tête (.hpp)
├── src/              # Fichiers source (.cpp)
├── docs/             # Documentation
├── tests/            # Tests unitaires
├── CMakeLists.txt    # Configuration CMake
└── Makefile          # Configuration Make (alternative)
```

## Modules Principaux

### Core Modules

#### 1. **Constants.hpp**
- Constantes du jeu (tailles de grille, nombre de joueurs, etc.)
- Énumérations (BonusType, TileShape)

#### 2. **Tile.hpp / Tile.cpp**
- Représentation des tuiles (96 types)
- Rotation (90°, 180°, 270°)
- Flipping (horizontal, vertical)
- Génération de toutes les orientations possibles

#### 3. **Board.hpp / Board.cpp**
- Gestion de la grille (20×20 ou 30×30)
- Placement de tuiles
- Suivi des territoires par joueur
- Gestion des cases bonus
- Gestion des pierres (blocage)

#### 4. **Player.hpp / Player.cpp**
- État du joueur (ID, nom, position de départ)
- Gestion des coupons d'échange
- Compteur de tuiles d'herbe placées

#### 5. **Queue.hpp / Queue.cpp**
- File d'attente des tuiles (96 tuiles distribuées séquentiellement)
- Système d'échange (prévisualisation de 5 tuiles)
- Recyclage des tuiles échangées

#### 6. **Game.hpp / Game.cpp**
- Boucle principale du jeu
- Gestion des rounds (9 rounds)
- Orchestration de tous les modules
- Conditions de victoire

### Utility Modules

#### 7. **Validator.hpp / Validator.cpp**
- Validation des placements de tuiles
- Vérification des règles :
  - Première tuile touche la tuile de départ
  - Tuiles suivantes touchent le territoire
  - Pas de chevauchement
  - Pas de contact avec les ennemis

#### 8. **Bonus.hpp / Bonus.cpp**
- Gestion des cases bonus :
  - **Tile Exchange** : 1.5 par joueur (arrondi)
  - **Stone** : 0.5 par joueur (arrondi)
  - **Robbery** : 1 par joueur
- Activation des bonus

#### 9. **Algorithms.hpp / Algorithms.cpp**
- **findLargestSquare()** : Détection du plus grand carré (Dynamic Programming)
- **isConnectedComponent()** : Vérification de connectivité (BFS/DFS)
- **getConnectedTerritory()** : Calcul du territoire connecté

#### 10. **Display.hpp / Display.cpp**
- Rendu CLI du plateau de jeu
- Affichage des informations des joueurs
- Visualisation des tuiles et options

#### 11. **InputHandler.hpp / InputHandler.cpp**
- Gestion de l'input utilisateur
- Saisie des positions
- Sélection des orientations
- Menus interactifs

## Flux de Données

```
main.cpp
  └──> Game::run()
       ├──> initializeGame()
       ├──> Boucle 9 rounds
       │    ├──> processPlayerTurn()
       │    │    ├──> Queue::getNext() ou exchangeTile()
       │    │    ├──> Validator::isValidPlacement()
       │    │    ├──> Board::placeTile()
       │    │    └──> BonusManager::processBonusCapture()
       ├──> purchaseSmallTiles()
       └──> findWinner() -> Algorithms::findLargestSquare()
```

## Algorithmes Clés

### 1. Détection du Plus Grand Carré
**Complexité : O(n²)**
- Utilise Dynamic Programming
- Pour chaque position, calcule la taille du plus grand carré possible
- Retourne la position et la taille du carré maximal

### 2. Connectivité du Territoire
**Complexité : O(n)**
- Utilise BFS/DFS
- À partir de la tuile de départ, explore toutes les tuiles connectées
- Vérifie que le territoire forme un composant connecté

### 3. Validation de Placement
**Complexité : O(k) où k = taille de la tuile**
- Vérifie limites de la grille
- Vérifie connexion au territoire
- Vérifie absence de chevauchement
- Vérifie absence de contact ennemi
- Vérifie conflits avec pierres

## Points d'Extension

1. **Sauvegarde/Chargement** : Serialization des états du jeu
2. **IA** : Stratégies automatiques pour les joueurs
3. **Réseau** : Mode multijoueur en réseau
4. **GUI** : Interface graphique (optionnel)

## Dépendances

- C++17 minimum
- Pas de dépendances externes requises (CLI pur)

## Compilation

```bash
# Avec CMake
mkdir build && cd build
cmake ..
make

# Ou avec Makefile
make
```


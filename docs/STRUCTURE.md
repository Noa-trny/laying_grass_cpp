# Structure Complète du Projet

## Arborescence des Fichiers

```
laying_grass_cpp/
│
├── include/                      # En-têtes C++ (.hpp)
│   ├── Constants.hpp             # Constantes du jeu
│   ├── Position.hpp              # Structure Position
│   ├── Tile.hpp                  # Classe Tile (96 types)
│   ├── Board.hpp                 # Classe Board (grille)
│   ├── Player.hpp                # Classe Player
│   ├── Queue.hpp                 # File d'attente des tuiles
│   ├── Game.hpp                  # Classe Game (logique principale)
│   ├── Validator.hpp             # Validation des placements
│   ├── Bonus.hpp                 # Gestion des bonus
│   ├── Algorithms.hpp            # Algorithmes (carré, connectivité)
│   ├── Display.hpp               # Affichage CLI
│   └── InputHandler.hpp          # Gestion de l'input
│
├── src/                          # Sources C++ (.cpp)
│   ├── main.cpp                  # Point d'entrée
│   ├── Tile.cpp                  # Implémentation Tile
│   ├── Board.cpp                 # Implémentation Board
│   ├── Player.cpp                # Implémentation Player
│   ├── Queue.cpp                 # Implémentation Queue
│   ├── Game.cpp                  # Implémentation Game
│   ├── Validator.cpp             # Implémentation Validator
│   ├── Bonus.cpp                 # Implémentation Bonus
│   ├── Algorithms.cpp            # Implémentation Algorithms
│   ├── Display.cpp               # Implémentation Display
│   └── InputHandler.cpp          # Implémentation InputHandler
│
├── docs/                         # Documentation
│   ├── ARCHITECTURE.md           # Architecture détaillée
│   ├── DESIGN.md                 # Design et algorithmes
│   ├── STRUCTURE.md              # Ce fichier
│   └── USER_GUIDE.md             # Guide utilisateur (à créer)
│
├── tests/                        # Tests unitaires
│   ├── test_tile.cpp
│   ├── test_board.cpp
│   ├── test_queue.cpp
│   ├── test_validator.cpp
│   └── test_algorithms.cpp
│
├── CMakeLists.txt                # Configuration CMake
├── Makefile                      # Configuration Make
├── README.md                     # Documentation principale
└── .gitignore                    # Fichiers à ignorer
```

## Dépendances entre Modules

```
main.cpp
  └──> Game
       ├──> Board
       │    ├──> Position
       │    ├──> Tile
       │    └──> Constants
       ├──> Player
       │    └──> Position
       ├──> Queue
       │    └──> Tile
       ├──> Validator
       │    ├──> Board
       │    ├──> Tile
       │    └──> Player
       ├──> BonusManager
       │    ├──> Board
       │    ├──> Player
       │    └──> Constants
       └──> Algorithms
            └──> Board
```

## Ordre de Développement Recommandé

### Phase 1 : Fondations
1. ✅ **Constants.hpp** - Définir toutes les constantes
2. ✅ **Position.hpp** - Structure de base
3. **Tile.cpp** - Implémentation des tuiles et transformations
4. **Board.cpp** - Grille et placement basique

### Phase 2 : Système de Jeu
5. **Player.cpp** - Gestion des joueurs
6. **Queue.cpp** - File d'attente avec échange
7. **Validator.cpp** - Validation des règles

### Phase 3 : Fonctionnalités Avancées
8. **Algorithms.cpp** - Détection carré + connectivité
9. **Bonus.cpp** - Système de bonus
10. **Display.cpp** - Interface CLI
11. **InputHandler.cpp** - Gestion input

### Phase 4 : Intégration
12. **Game.cpp** - Boucle principale
13. **main.cpp** - Point d'entrée complet

### Phase 5 : Tests
14. Tests unitaires pour chaque module
15. Tests d'intégration

## Interfaces Publiques

### Tile
- `Tile(int id, shape)` - Constructeur
- `rotate90/180/270()` - Rotations
- `flipHorizontal/Vertical()` - Retournements
- `getAllOrientations()` - Toutes les variantes

### Board
- `placeTile(tile, pos, playerId)` - Placement
- `canPlaceTile(tile, pos, playerId)` - Validation
- `getTerritory(playerId)` - Territoire d'un joueur
- `placeStone(pos)` / `removeStone(pos)` - Gestion pierres

### Queue
- `getNext()` - Prochaine tuile
- `peekNext(count)` - Prévisualiser
- `exchangeTile(index)` - Échanger

### Validator
- `isValidPlacement(tile, pos, player, isFirst)` - Validation complète

### Algorithms
- `findLargestSquare(board, playerId)` - Plus grand carré
- `isConnectedComponent(board, playerId, territory)` - Connectivité

## Points de Configuration

### Dans Constants.hpp
- `MIN_PLAYERS = 2`
- `MAX_PLAYERS = 9`
- `SMALL_GRID_SIZE = 20`
- `LARGE_GRID_SIZE = 30`
- `TOTAL_TILE_TYPES = 96`
- `EXCHANGE_PREVIEW_SIZE = 5`

### Dans Game.cpp
- Nombre de rounds : `TILES_PER_PLAYER = 9`
- Calcul bonus : `1.5 * numPlayers` (Tile Exchange), `0.5 * numPlayers` (Stone)


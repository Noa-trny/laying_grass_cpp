# Structure du dépôt

## 1. Arborescence

```
laying_grass_cpp/
├── CMakeLists.txt             # Build principal (jeu + tests)
├── Makefile                   # Build de secours
├── README.md                  # Présentation rapide du projet
├── data/
│   └── tiles.json             # Définitions des 96 formes de tuiles
├── docs/
│   ├── DESIGN.md              # Design détaillé et algorithmes
│   ├── STRUCTURE.md           # Présent document
│   ├── USER_DOCUMENTATION.md  # Guide d’utilisation pour la CLI
│   └── TECHNICAL_DOCUMENTATION.md # Choix d’implémentation et détails techniques
├── include/                   # Interfaces publiques (headers)
│   ├── Algorithms.hpp
│   ├── Board.hpp
│   ├── Bonus.hpp
│   ├── ConsoleUtils.hpp
│   ├── Constants.hpp
│   ├── Game.hpp
│   ├── GameUtils.hpp
│   ├── InputHandler.hpp
│   ├── Player.hpp
│   ├── Position.hpp
│   ├── Queue.hpp
│   ├── Tile.hpp
│   ├── TileParser.hpp
│   └── Validator.hpp
├── src/                       # Implémentations (.cpp)
│   ├── Algorithms.cpp
│   ├── Board.cpp
│   ├── Bonus.cpp
│   ├── ConsoleUtils.cpp
│   ├── Game.cpp
│   ├── GameUI.cpp             # Fonctions d’affichage/interaction
│   ├── GameUtils.cpp
│   ├── InputHandler.cpp
│   ├── Queue.cpp
│   ├── Tile.cpp
│   ├── TileParser.cpp
│   └── main.cpp
└── tests/
    └── bonus_tests.cpp        # Suite de tests (LayingGrass_tests)
```

Deux exécutables sont générés :

- `LayingGrass` : jeu interactif.
- `LayingGrass_tests` : tests unitaires (bonus). D’autres suites peuvent être ajoutées dans `tests/`.

## 2. Dépendances entre modules

```
main.cpp
 └── Game
     ├─ Board ──┬─ Tile
     │          └─ Position
     ├─ Queue ──┬─ Tile
     │          └─ TileParser (JSON)
     ├─ Player ─┬─ Position
     ├─ Validator ──┬─ Board
     │              └─ Player
     ├─ BonusManager ──┬─ Board
     │                 └─ Player
     ├─ Algorithms ──┬─ Board
     ├─ GameUtils / GameUI
     ├─ InputHandler
     └─ ConsoleUtils
```

Les dépendances sont acycliques : `Game` reste l’orchestrateur unique. `Board`, `Queue`, `Validator` et `BonusManager` ne connaissent pas l’UI (tout passe par `Game`).

## 3. Description rapide des dossiers

### include/ et src/
- **Algorithms** : fonctions stateless (plus grand carré, connectivité).
- **Board** : noyau de la mécanique de plateau ; assure la cohérence des grilles et expose des helpers pour les tests (ex. `setBonusSquare`).
- **Bonus** : logique d’activation des bonus ; dépend de `InputHandler` pour les choix interactifs.
- **ConsoleUtils** : effacement d’écran, couleurs ANSI (couplé à `GameUI`).
- **Game** : boucle principale, gestion des tours, phase finale.
- **GameUI / GameUtils** : rendu console, overlay, légende ; aucun état propre.
- **InputHandler** : lecture et validation des entrées ; accepte lettres ou chiffres pour les coordonnées.
- **Player** : état individuel (coupons, couleurs, nombre de cases).
- **Queue** : file de tuiles avec mécanisme d’échange/recyclage.
- **Tile** / **TileParser** : représentation des tuiles et chargement JSON.
- **Validator** : règles métier de placement.

### docs/
- Documentation fonctionnelle et technique. Chaque fichier est à jour et cohérent avec l’implémentation actuelle.

### tests/
- `bonus_tests.cpp` : couvre la capture de bonus (échange, pierre, vol). Les entrées sont simulées via redirection de `std::cin`.
- Pour créer un nouveau test, ajouter un fichier dans `tests/` et lister l’exécutable dans `CMakeLists.txt`.

## 4. Cibles CMake

```cmake
add_executable(LayingGrass         ${SOURCES_APP})
add_executable(LayingGrass_tests   ${SOURCES_COMMON} tests/bonus_tests.cpp)
```

- `SOURCES_COMMON` regroupe tous les `.cpp` (hors `main.cpp`).
- Les tests réutilisent les mêmes modules métier ; l’UI n’est pas nécessaire.
- Les commandes de compilation et d’exécution sont décrites dans le `README.md`.

## 5. Répertoires générés (build)

Après compilation (via CMake en mode Debug), on obtient typiquement :

```
cmake-build-debug/
├── CMakeCache.txt
├── CMakeFiles/
├── LayingGrass.exe
├── LayingGrass_tests.exe
└── ...
```

Ces répertoires sont ignorés par Git et peuvent être supprimés (`cmake --build build --target clean`).

## 6. Bonnes pratiques

- Chaque module possède son header et son implémentation. Ajouter une fonctionnalité implique de modifier l’un de ces couples.
- Les scripts CLI (`InputHandler`, `ConsoleUtils`) ne doivent pas contenir de logique métier (pour faciliter d’éventuelles évolutions vers une GUI).
- Les tests doivent manipuler `Board`, `Player`, `BonusManager`, `Queue`, etc. directement. Préférer le redirigement de `std::cin` pour simuler les choix utilisateur lorsqu’on dépend d’`InputHandler`.

Cette vue structurelle sert de guide pour naviguer dans le dépôt, ajouter des fonctionnalités ou créer de nouveaux tests.


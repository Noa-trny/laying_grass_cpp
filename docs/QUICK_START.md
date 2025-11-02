# Quick Start Guide

## Vue d'Ensemble Rapide

Architecture modulaire C++ pour le jeu Laying Grass avec séparation claire des responsabilités.

## Modules Clés

| Module | Responsabilité | Complexité |
|--------|---------------|------------|
| **Game** | Orchestration principale | ⭐⭐⭐ |
| **Board** | Gestion grille & territoires | ⭐⭐⭐ |
| **Tile** | Représentation & transformations | ⭐⭐ |
| **Queue** | File d'attente avec échange | ⭐⭐ |
| **Validator** | Validation des règles | ⭐⭐⭐ |
| **Algorithms** | Plus grand carré, connectivité | ⭐⭐⭐⭐ |
| **Bonus** | Cases bonus | ⭐⭐ |
| **Display** | Interface CLI | ⭐ |
| **InputHandler** | Saisie utilisateur | ⭐ |

## Compilation Rapide

```bash
# Option 1: CMake
mkdir build && cd build
cmake ..
make

# Option 2: Makefile direct
make
```

## Structure Minimale pour Démarrer

### Fichiers Prioritaires à Implémenter

1. **Position.hpp/cpp** - Structure de base ✅
2. **Constants.hpp** - Configuration ✅
3. **Tile.hpp/cpp** - Fondation des tuiles
4. **Board.hpp/cpp** - Grille basique
5. **Game.hpp/cpp** - Boucle minimale

### Ordre d'Implémentation Minimal

```
Week 1: Foundation
├── Day 1-2: Tile + Board basique
├── Day 3-4: Player + Queue
└── Day 5-7: Validator + Game loop basique

Week 2: Features
├── Day 8-9: Algorithms (carré, connectivité)
├── Day 10: Bonus system
└── Day 11: Display + Input + Tests
```

## Points Critiques

⚠️ **Ne pas oublier** :
- Queue avec recyclage des tuiles échangées
- Validation stricte (première tuile, connectivité)
- Détection bonus sur 4 directions
- Plus grand carré peut être non-contigu

## Checklist Implémentation

- [ ] Tile avec rotation/flip fonctionnels
- [ ] Board avec placement basique
- [ ] Queue avec système d'échange
- [ ] Validator avec toutes les règles
- [ ] Algorithms::findLargestSquare()
- [ ] Bonus capture et activation
- [ ] Display CLI lisible
- [ ] InputHandler robuste
- [ ] Game loop complète (9 rounds)
- [ ] Phase achat tuiles 1×1
- [ ] Calcul gagnant + tiebreaker
- [ ] Tests unitaires

## Commandes Utiles

```bash
# Compiler
make

# Nettoyer
make clean

# Lancer
./laying_grass

# Tests (à configurer)
make test
```

## Documentation Complète

- `ARCHITECTURE.md` - Architecture détaillée
- `DESIGN.md` - Design et algorithmes
- `STRUCTURE.md` - Structure des fichiers
- `IMPLEMENTATION_NOTES.md` - Notes techniques


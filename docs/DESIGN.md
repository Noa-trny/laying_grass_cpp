# Design Document - Laying Grass Game

## Diagramme d'Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                         main.cpp                             │
│                    Point d'entrée                            │
└────────────────────────┬────────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────┐
│                         Game                                 │
│  - Boucle principale (9 rounds)                             │
│  - Gestion des tours                                        │
│  - Conditions de victoire                                    │
└───┬──────────┬──────────┬──────────┬──────────┬────────────┘
    │          │          │          │          │
    ▼          ▼          ▼          ▼          ▼
┌─────────┐ ┌──────┐ ┌───────┐ ┌──────────┐ ┌─────────┐
│ Board   │ │Queue │ │Player │ │Validator │ │ Bonus   │
│         │ │      │ │       │ │          │ │ Manager │
│ - Grille│ │ - 96 │ │ - ID  │ │ - Rules  │ │ - Tile  │
│ - Bonus │ │ tiles│ │ - Cou-│ │ - Valid. │ │   Exch. │
│ - Stones│ │ - Ex-│ │   pons│ │          │ │ - Stone │
│ - Terr. │ │ change│ │       │ │          │ │ - Robb. │
└────┬────┘ └──────┘ └───────┘ └──────────┘ └─────────┘
     │
     ▼
┌─────────────────────────────────────────────────────────────┐
│                         Tile                                 │
│  - 96 types                                                  │
│  - Rotation (90°, 180°, 270°)                               │
│  - Flip (H, V)                                               │
│  - Toutes orientations                                       │
└─────────────────────────────────────────────────────────────┘
     │
     ▼
┌─────────────────────────────────────────────────────────────┐
│                      Algorithms                              │
│  - findLargestSquare() (DP O(n²))                           │
│  - isConnectedComponent() (BFS/DFS O(n))                    │
│  - getConnectedTerritory()                                  │
└─────────────────────────────────────────────────────────────┘
```

## Flux d'Exécution

```
1. Initialisation
   ├──> Saisie nombre de joueurs (2-9)
   ├──> Création Board (20×20 ou 30×30)
   ├──> Initialisation Queue (96 tuiles mélangées)
   ├──> Placement tuiles de départ (1×1 par joueur)
   └──> Placement cases bonus aléatoires

2. Boucle Principale (9 rounds)
   └──> Pour chaque joueur :
       ├──> Obtenir tuile (Queue ou échange)
       ├──> Saisie position et orientation
       ├──> Validation (Validator)
       ├──> Placement (Board::placeTile)
       ├──> Vérification bonus capture
       └──> Mise à jour territoire

3. Phase d'Achat
   └──> Chaque joueur peut acheter tuiles 1×1

4. Calcul du Gagnant
   ├──> Algorithms::findLargestSquare() pour chaque joueur
   ├──> Comparaison des carrés
   └──> Tiebreaker : plus de tuiles d'herbe

5. Affichage Résultats
```

## Structures de Données Clés

### Tile
```cpp
- id: int
- shape: vector<vector<bool>> (matrice)
- width, height: int
- Méthodes: rotate, flip, getAllOrientations
```

### Board
```cpp
- grid: vector<vector<int>> (joueur ID par cellule)
- bonusGrid: vector<vector<bool>>
- bonusTypes: vector<vector<BonusType>>
- stoneGrid: vector<vector<bool>>
- playerTerritories: map<int, set<Position>>
```

### Player
```cpp
- id: int
- name: string
- startPosition: Position
- exchangeCoupons: int
- grassTilesPlaced: int
```

### Queue
```cpp
- mainQueue: vector<Tile> (96 tuiles)
- exchangeQueue: vector<Tile> (tuiles échangées)
- currentIndex: size_t
```

## Algorithmes Détaillés

### 1. findLargestSquare()
**Objectif** : Trouver le plus grand carré dans le territoire d'un joueur

**Algorithme** : Dynamic Programming
```
Pour chaque position (i, j):
  si cellule(i,j) appartient au joueur:
    dp[i][j] = min(dp[i-1][j], dp[i][j-1], dp[i-1][j-1]) + 1
  sinon:
    dp[i][j] = 0

Résultat = max(dp[i][j]) pour tout i, j
```

**Complexité** : O(n²) où n = taille de la grille

### 2. isConnectedComponent()
**Objectif** : Vérifier que le territoire est connecté

**Algorithme** : BFS depuis la tuile de départ
```
Queue q
Set visited
q.push(startPosition)
visited.add(startPosition)

Tant que q non vide:
  pos = q.pop()
  Pour chaque voisin de pos:
    si voisin appartient au territoire et non visité:
      q.push(voisin)
      visited.add(voisin)

Résultat = (visited.size() == territory.size())
```

**Complexité** : O(k) où k = nombre de cellules du territoire

### 3. Validation de Placement
**Vérifications** (dans l'ordre) :
1. Limites de la grille : toutes les cellules de la tuile dans [0, size-1]
2. Première tuile : touche startPosition
3. Tuiles suivantes : touche le territoire (au moins 1 côté)
4. Pas de chevauchement : pas de cellule déjà occupée
5. Pas de contact ennemi : pas de cellule adjacente à un ennemi
6. Pas de conflit pierre : pas de pierre sur la zone

## Gestion des Bonus

### Tile Exchange Square
- Placement : 1.5 par joueur (arrondi supérieur)
- Activation : Capturer avec tuile sur les 4 directions
- Effet : Ajoute 1 coupon d'échange

### Stone Square
- Placement : 0.5 par joueur (arrondi supérieur)
- Activation : Capturer avec tuile
- Effet : Permet placer une pierre (bloque placement)
- Coût retrait : 1 coupon

### Robbery Square
- Placement : 1 par joueur
- Activation : Capturer avec tuile
- Effet : Vole une tuile d'un autre joueur

## Points d'Attention

1. **Queue avec Échange** : Les tuiles échangées doivent revenir après épuisement de la queue principale
2. **Connectivité** : Le territoire doit rester connecté à la tuile de départ
3. **Plus Grand Carré** : Peut être non-contigu si le territoire est complexe
4. **Tiebreaker** : Si carrés égaux, compter toutes les tuiles d'herbe (pas seulement le carré)


# Notes d'Implémentation

## Défis Techniques Principaux

### 1. Génération des 96 Tuiles
**Problème** : Créer 96 types de tuiles uniques

**Solution** :
- Définir des patterns de base (shapes)
- Générer automatiquement toutes les orientations
- Utiliser un système d'ID unique pour éviter les doublons
- Stocker les tuiles dans un fichier de configuration ou générer algorithmiquement

**Implémentation suggérée** :
```cpp
class TileGenerator {
    static std::vector<Tile> generateAllTiles() {
        // Patterns de base
        // Génération des variantes
        // Retour de 96 tuiles uniques
    }
};
```

### 2. Détection du Plus Grand Carré
**Problème** : Le carré peut ne pas être contigu dans un territoire complexe

**Solution** :
- Algorithme Dynamic Programming classique pour trouver tous les carrés possibles
- Filtrer pour ne garder que ceux entièrement dans le territoire
- Gérer le cas où le territoire n'est pas rectangulaire

**Attention** : Le carré doit être composé uniquement de cellules du territoire.

### 3. Gestion de la Queue avec Échange
**Problème** : Les tuiles échangées doivent revenir dans la queue après épuisement

**Solution** :
```cpp
class Queue {
    std::vector<Tile> mainQueue;
    std::vector<Tile> exchangeQueue;  // Tuiles échangées
    size_t currentIndex;
    
    Tile getNext() {
        if (currentIndex >= mainQueue.size()) {
            // Recycler exchangeQueue dans mainQueue
            mainQueue = exchangeQueue;
            shuffle(mainQueue);
            exchangeQueue.clear();
            currentIndex = 0;
        }
        return mainQueue[currentIndex++];
    }
};
```

### 4. Validation de Connectivité
**Problème** : Vérifier qu'une tuile touche bien le territoire connecté

**Solution** :
- Utiliser BFS depuis la tuile de départ
- Vérifier qu'au moins une cellule de la nouvelle tuile est adjacente à une cellule du territoire
- Adjacence = même ligne/colonne (pas diagonale)

### 5. Détection de Capture de Bonus
**Problème** : Bonus activé quand tuile placée sur les 4 directions cardinales

**Solution** :
```cpp
bool checkBonusCapture(int row, int col) {
    // Vérifier Nord, Sud, Est, Ouest
    bool north = hasPlayerTile(row-1, col, playerId);
    bool south = hasPlayerTile(row+1, col, playerId);
    bool east = hasPlayerTile(row, col+1, playerId);
    bool west = hasPlayerTile(row, col-1, playerId);
    
    return north && south && east && west;
}
```

### 6. Gestion des Pierres (Stones)
**Problème** : Les pierres bloquent les placements mais peuvent être retirées avec un coupon

**Solution** :
- `stoneGrid[row][col] = true` pour marquer une pierre
- Dans `canPlaceTile()`, vérifier `!stoneGrid[r][c]`
- Méthode `removeStone()` qui consomme un coupon

### 7. Achat de Tuiles 1×1
**Problème** : Après les 9 rounds, permettre l'achat de tuiles 1×1

**Solution** :
- Créer une tuile spéciale 1×1 (une seule cellule true)
- Menu d'achat dans `Game::purchaseSmallTiles()`
- Utiliser des coupons comme monnaie (ou système séparé)

## Choix de Design

### Structure de Données pour la Grille
**Choix** : `std::vector<std::vector<int>>` où valeur = ID joueur (0 = vide)

**Avantages** :
- Accès O(1)
- Simple à implémenter
- Facile à visualiser

**Alternative** : `std::map<Position, int>` pour grilles creuses (non nécessaire ici)

### Représentation des Tuiles
**Choix** : `std::vector<std::vector<bool>>` pour la forme

**Avantages** :
- Flexible (tuiles de différentes tailles)
- Facile à transformer (rotation/flip)
- Simple à comparer

### Territoire des Joueurs
**Choix** : `std::map<int, std::set<Position>>`

**Avantages** :
- Recherche rapide O(log n)
- Pas de doublons
- Facile à itérer

**Alternative** : `std::map<int, std::vector<Position>>` (plus simple mais avec doublons possibles)

## Optimisations Possibles

1. **Cache des Orientations** : Pré-calculer toutes les orientations au chargement
2. **Bitmask pour Tuiles** : Utiliser bits au lieu de bool pour performance
3. **Spatial Indexing** : Pour validation rapide des chevauchements
4. **Memoization DP** : Cache des résultats de findLargestSquare()

## Tests Critiques

1. **Test Placement Première Tuile** : Doit toucher startPosition
2. **Test Placement Suivante** : Doit toucher territoire
3. **Test Chevauchenent** : Doit échouer si overlap
4. **Test Contact Ennemi** : Doit échouer si touche ennemi
5. **Test Connectivité** : Territoire doit rester connecté
6. **Test Queue Échange** : Tuiles échangées reviennent après
7. **Test Bonus Capture** : Activation sur 4 directions
8. **Test Plus Grand Carré** : Cas simples et complexes
9. **Test Tiebreaker** : Comptage correct des tuiles d'herbe

## Gestion des Erreurs

- **Placement Invalide** : Retourner false, skip le tour
- **Tuile Indisponible** : Gérer fin de queue
- **Input Invalide** : Boucle de saisie jusqu'à valide
- **Exception** : Try-catch dans main.cpp

## Performance Attendue

- **Initialisation** : < 1 seconde
- **Validation Placement** : < 10ms
- **Détection Plus Grand Carré** : < 100ms pour grille 30×30
- **Rendu CLI** : < 50ms
- **Tour Complet** : < 1 seconde (avec input)


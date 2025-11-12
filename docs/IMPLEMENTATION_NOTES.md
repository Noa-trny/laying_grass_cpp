# Notes d'Implémentation

## 1. Chargement des données

- **Tiles** : `TileParser::loadTilesFromJson()` parcourt `data/tiles.json` sans dépendance externe.
- **Constantes** : `Constants.hpp` définit les bornes (joueurs, tailles de grille) et le ratio de tuiles par joueur (`10.67`).

## 2. Structures principales

### Board
- `grid` (`int`) : propriétaire de chaque case, `0` = vide.
- `bonusGrid`, `bonusTypes` : cases bonus et type affecté.
- `stoneGrid` : cases occupées par une pierre.
- `playerTerritories` : ensemble ordonné des positions contrôlées.
- Helpers : `isInside`, `isOwnedBy`, `claimBonusSquare`.

### Player
- Identité, couleur (nom + code ANSI), coupons disponibles.
- `grassSquaresOwned` suit le nombre exact de cases contrôlées (utilisé pour le tiebreaker).

### Queue
- Sélectionne un sous-ensemble unique des 96 tuiles.
- Nombre calculé = `round(numPlayers * ratio)` mais au moins `9 * numPlayers`.
- `exchangeQueue` conserve les tuiles utilisées pour l’échange afin de les réinsérer dans l’ordre.

## 3. Boucle de jeu (`Game`)

1. **Initialisation**
   - Saisie noms + couleurs (`InputHandler::chooseColor`).
   - Mélange de l’ordre de jeu (Mersenne Twister).
   - Placement interactif des tuiles de départ.
   - Calcul des tours max (min(9, totalTiles / joueurs)).

2. **Tour**
   - Menu : tirer, échanger, retirer pierre.
   - Gestion coupons dans `Player`.
   - Transformations sur la tuile choisie.
   - Validation via `Validator::isValidPlacement`.
   - Mise à jour du territoire et détection bonus.

3. **Phase finale**
   - Achat de tuiles 1×1, validation via le même moteur.
   - Déterminer gagnant : `Algorithms::findLargestSquare`, puis `grassSquaresOwned`.

## 4. Validation des règles

`Validator` encapsule la logique :
- `isWithinBounds` : limites plateau.
- `noOverlap` : aucune case occupée (hors joueur).
- `noStoneConflict` : aucune pierre sous la tuile.
- `noEnemyContact` : pas de contact orthogonal avec un autre joueur.
- `touchesTerritory` : continuité du territoire quand `isFirstTile == false`.

Les règles de base (`Board::canPlaceTile`) ne vérifient que les contraintes physiques ; `Validator` ajoute les règles de gameplay.

## 5. Bonus

- Distribution pseudo-aléatoire (évitant bords et clusters).
- Capture : `Board::checkBonusCapture` vérifie les quatre directions.
- Application : `BonusManager::processBonusCapture` déclenche l’effet et revendique la case bonus.
  - Échange : +1 coupon.
  - Pierre : placement interactif, contrôle de validité.
  - Vol : sélection d’une case adverse disponible (1 case transférée).

## 6. Interface et saisie

- `InputHandler` accepte lettres ou chiffres (colonne AA == 27).
- Ré-affichage après actions pour limiter le “ghosting” CLI.
- Les couleurs sont stockées dans `ConsoleUtils::colorOptions`.

## 7. Points techniques spécifiques

- **Rotation/miroirs** : `Tile` calcule directement les matrices transformées.
- **Recherche du plus grand carré** : `Algorithms::findLargestSquare` (DP classique).
- **Territoire** : `Board::addTerritory/removeTerritory` maintient la cohérence pour bonus et scoring.
- **Défausse** : les tuiles abandonnées ne sont pas remises dans la file (conformément à la règle).
- **Retrait pierre** : nécessite un coupon et actualise l’affichage du tour.

## 8. Limites et TODO connus

- Vol de tuile : transfert case par case (pas de reconstruction d’une tuile multi-case).
- Pas de persistance de partie.
- Pas de tests unitaires automatisés intégrés (prévoir `CTest` ou `Catch2`).

## 9. Conseils de maintenance

- Centraliser les messages d’erreur dans `InputHandler` pour homogénéiser l’UX.
- Ajouter des tests sur `Validator` (placements limites, adjacency).
- Envisager de sérialiser l’état pour rejouer une partie (fichier JSON).
- Documenter `tiles.json` si de nouvelles formes sont ajoutées.

## 10. Commandes de développement

```bash
# Build complet
cmake -S . -B build
cmake --build build --config Release

# Exécution
./build/laying_grass

# Nettoyage
cmake --build build --target clean
```



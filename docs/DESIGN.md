# Document de conception – *Laying Grass*

## 1. Rappels de gameplay

- 2 à 9 joueurs.
- Plateau 20×20 (≤ 4 joueurs) ou 30×30 (≥ 5 joueurs).
- Chaque joueur possède une tuile de départ 1×1 posée manuellement.
- À son tour :
  1. prendre la prochaine tuile ou échanger contre l’une des cinq suivantes (1 coupon), ou retirer une pierre (1 coupon) ;
  2. transformer la tuile (rotations, miroirs) ;
  3. sélectionner une position et prévisualiser le placement ;
  4. valider ou annuler ;
  5. en cas de réussite, mettre à jour le territoire et appliquer les bonus ; sinon réessayer ou défausser.
- Une tuile défaussée n’est pas remise dans la file.
- Après 9 tours maximum par joueur : possibilité d’acheter des tuiles 1×1 avec les coupons restants.
- Victoire : plus grand carré de territoire ; égalité départagée au nombre total de cases possédées.

## 2. Objectifs de game design

- Offrir des décisions tactiques à chaque tour (piocher, échanger, retirer une pierre, poser ou défausser).
- Encourager la projection à moyen terme avec la contrainte du plus grand carré.
- Maintenir une tension sur les ressources : coupons d’échange limités, pierres imposées par les bonus.
- Préserver la lisibilité de l’état du plateau en CLI (prévisualisation, légende, résumé de tour).

## 3. Cycle d’exécution détaillé

```
Game::Game()
 ├─ initialise les joueurs (noms, couleurs, ordre aléatoire)
 ├─ configure la file Queue (ratio 10,67 tuiles/joueur, min 9)
 ├─ initialise le plateau (bonus aléatoires, pierres vides)
 └─ initStartingTiles() : chaque joueur pose sa tuile de départ

Game::run()
 ├─ while running && Queue::hasNext() && currentRound < 9
 │   ├─ Player current = players[currentPlayerIndex]
 │   └─ handlePlayerTurn(current)
 │       ├─ menu (tirer/échanger/retirer pierre)
 │       ├─ InputHandler pour rotation & position
 │       ├─ Validator::isValidPlacement()
 │       ├─ Board::placeTile() + Board::addTerritory()
 │       └─ BonusManager::processBonusCapture()
 │   └─ advanceTurn()
 ├─ processFinalPhase() : achat tuiles 1×1
 └─ determineWinner() + showSummary()
```

## 4. Bonus et effets

| Symbole | Nom        | Placement initial                              | Condition de capture                                        | Effet immédiat                                                            |
|---------|------------|-------------------------------------------------|-------------------------------------------------------------|---------------------------------------------------------------------------|
| `E`     | Échange    | 1,5 case par joueur (arrondi supérieur)        | Les 4 cases cardinales appartiennent au joueur              | +1 coupon d’échange.                                                      |
| `P`     | Pierre     | 0,5 case par joueur (arrondi supérieur)        | Idem ; **si recouverte directement, bonus perdu**           | Le joueur doit placer une pierre sur un emplacement libre.                |
| `R`     | Vol        | 1 case par joueur                              | Idem                                                        | Choisir un adversaire et lui voler une case (territoire + score).         |

Les bonus capturés sont enregistrés par `Board::claimBonusSquare` puis `BonusManager::activate…`. Une case bonus recouverte directement est simplement perdue (`Board::placeTile` supprime le drapeau `bonusGrid`).

- Pour des détails d’implémentation (structures de données, règles de validation), se référer à `TECHNICAL_DOCUMENTATION.md`.

## 5. Points d’équilibrage

- **Tirage aléatoire vs. information** : la prévisualisation de cinq tuiles via l’échange doit rester un avantage conditionné au coupon.
- **Bonus pierre** : impose une contrainte négative pour compenser l’accès gratuit au bonus ; son placement doit rester libre pour garantir des décisions intéressantes.
- **Vol** : limiter la cible aux joueurs ayant un territoire non vide évite les tours « morts ».
- **Coupons** : un coupon initial par joueur crée un espace décisionnel dès le premier tour sans déséquilibrer les premiers joueurs.

## 6. pistes futures

- Mode « annulation » pour revenir en arrière avant validation définitive.
- Historique des coups pour la relecture.
- Export JSON de l’état pour charger/reprendre une partie.
- IA (heuristique simple sur meilleure position possible) ; l’architecture actuelle permet de plugger un module de décision au moment où `handlePlayerTurn` attend une saisie.

Ce design document est aligné avec l’implémentation actuelle et doit servir de référence pour la maintenance ou l’ajout de nouvelles fonctionnalités.


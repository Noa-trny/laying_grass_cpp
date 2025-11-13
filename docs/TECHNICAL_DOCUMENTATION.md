# TECHNICAL_DOCUMENTATION

## 1. Objectifs techniques

- offrir une relecture fidèle du jeu télévisé tout en restant jouable en console ;
- séparer la logique métier de l’interface pour faciliter les tests et une éventuelle migration vers une GUI ;
- garantir la cohérence des règles (territoires, bonus, capture) même en cas d’interactions complexes.

## 2. Architecture modulaire

| Module            | Rôle principal                                                                                     |
|-------------------|----------------------------------------------------------------------------------------------------|
| `Game`            | Orchestrateur : boucle principale, tours, phase finale, calcul du vainqueur.                       |
| `Board`           | État du plateau : grille de propriétaires, présence de bonus, pierres et territoires par joueur.   |
| `Queue`           | File pseudo-aléatoire de tuiles, gestion de l’aperçu (5 prochaines) et des échanges.               |
| `Player`          | Stockage de l’état individuel (coupons, couleur, cases contrôlées, position de départ).            |
| `Validator`       | Règles métier de placement : limites, chevauchement, pierres, contacts alliés/adverses.            |
| `BonusManager`    | Activation des bonus (`E`, `P`, `R`) avec interactions console.                                     |
| `Algorithms`      | Fonctions d’analyse : plus grand carré, connexité, adjacences.                                      |
| `InputHandler`    | Lecture robuste des entrées utilisateur (menus, coordonnées, confirmations).                       |
| `GameUtils`/`GameUI` | Affichage console (plateau, tuiles, superposition, résumés).                                     |
| `ConsoleUtils`    | Services transverses pour les couleurs ANSI et le nettoyage d’écran.                               |

Le découplage permet d’exécuter les tests unitaires sans dépendre de l’interface : `tests/bonus_tests.cpp` manipule les modules métier directement.

## 3. File de tuiles

- Les 96 formes sont chargées depuis `data/tiles.json`.
- Calcul du nombre de tuiles : `max(9 * joueurs, round(10.67 * joueurs))`, assurant au minimum neuf tours.
- Les tuiles sont mélangées (`std::shuffle`) pour éviter tout biais.
- Lors d’un échange :
  - les cinq prochaines tuiles sont affichées ;
  - la tuile choisie est retirée du futur flux, les précédentes rejoignent `exchangeQueue` pour être rejouées plus tard, en conservant l’ordre.

Cette approche reproduit la mécanique de « queue visible » du jeu original tout en restant déterministe pour les tests (le semis peut être fixé si besoin).

## 4. Règles de placement

`Validator::isValidPlacement` applique l’ordre suivant :

1. **Limites** : toutes les cases de la tuile doivent rester dans la grille (`Board::isInside`).
2. **Chevauchement** : aucune cellule occupée par une tuile ou une pierre (`grid` et `stoneGrid`).
3. **Contact adverse** : aucun voisin orthogonal n’appartient à un autre joueur.
4. **Connexion** :
   - première tuile : doit toucher la case de départ du joueur ;
   - sinon : doit toucher une case déjà contrôlée.

Chaque échec renvoie un message spécifique pour guider l’utilisateur. Cette granularité facilite aussi les tests ciblés.

## 5. Gestion du plateau et des bonus

- `Board` maintient trois grilles parallèles (propriétaires, bonus, pierres) plus une `std::map<int, std::set<Position>>` pour les territoires.
- Lorsqu’une tuile recouvre un bonus, celui-ci est supprimé (cas « perdu »). Seule une capture par encerclement déclenche `BonusManager::processBonusCapture`.
- Les offsets `{-1,0}`, `{1,0}`, `{0,-1}`, `{0,1}` suffisent car les règles n’utilisent pas de diagonales.
- `BonusManager` délègue les choix interactifs à `InputHandler`. La logique de modification de territoire reste centralisée dans `Board` pour préserver la cohérence.

## 6. Calcul du classement

- `Algorithms::findLargestSquare` utilise un DP classique `O(n²)` pour chaque joueur.
- `Game::determineWinner` applique deux critères :
  1. taille du plus grand carré ;
  2. nombre total de cases contrôlées (départage exact des égalités).
- Cette méthode est robuste même si aucun carré n’existe : le second critère prend alors le relais automatiquement.

## 7. Interactions console

- Les couleurs ANSI sont activées sous Windows via `SetConsoleMode` (`ENABLE_VIRTUAL_TERMINAL_PROCESSING`), et fonctionnent nativement sur les autres plateformes.
- `ConsoleUtils::clearConsole` combine un effacement ANSI et un appel `cls` en dernier recours pour éviter les artefacts.
- `GameUtils::showBoardWithOverlay` permet de prévisualiser la pose avant validation, améliorant l’expérience utilisateur tout en servant de point d’ancrage pour une future interface graphique.

## 8. Tests et extensibilité

- `tests/bonus_tests.cpp` couvre les trois bonus en simulant les entrées standard pour assurer la stabilité des choix interactifs.
- De nouveaux tests peuvent être ajoutés en réutilisant les headers publics (`include/`), ce qui évite de dupliquer la logique métier.
- Les principaux points d’extension identifiés :
  - ajout d’un mode IA en branchant un module de décision dans `handlePlayerTurn` ;
  - sérialisation de l’état du plateau via `Board::playerTerritories` et la grille principale ;
  - interface graphique en remplaçant les appels console par des signaux/slots ou un moteur de rendu externe.

## 9. Décisions notables

- **Séparation UI / logique** : `GameUI` contient uniquement la présentation ; toute mutation d’état reste dans `Game`, `Board`, `Player`, etc.
- **Structure des positions** : `Position` est un type ordonné (`operator<`) afin de s’insérer dans les `std::set` sans adaptateur supplémentaire.
- **Coupons d’échange** : stockés côté joueur plutôt que globalement, ce qui facilite la mise en place d’effets individuels (bonus, phase finale).
- **Résolution des égalités** : implémentée dans `Game.cpp` pour refléter précisément les règles officielles, avec sauvegarde du détail dans le résumé final.

Ces choix ont pour but de maintenir un code lisible, testable et prêt pour des évolutions ultérieures, sans sacrifier la fidélité au jeu d’origine.



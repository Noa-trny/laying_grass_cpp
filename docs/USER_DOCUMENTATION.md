# USER_DOCUMENTATION

## 1. Vue d’ensemble

Laying Grass est un jeu de placement de tuiles compétitif inspiré de l’épreuve « Laying Grass » de *The Devil’s Plan*. Chaque joueuse ou joueur cherche à construire le plus grand carré connecté à sa couleur sur un plateau commun. La partie se déroule sur neuf tours, en mélangeant tirages aléatoires et stratégie de positionnement.

## 2. Lancer une partie

1. Ouvrir un terminal dans le dossier du projet.
2. Exécuter le binaire `LayingGrass` (par exemple `./LayingGrass` sous Linux/macOS ou `LayingGrass.exe` sous Windows).
3. Répondre aux questions de configuration :
   - nombre de joueurs (entre 2 et 9) ;
   - nom de chaque joueur (une valeur par défaut est proposée) ;
   - choix de la couleur dans la palette disponible (un code couleur ANSI est appliqué dans la console).
4. Chaque joueur place sa tuile de départ 1×1 sur une case libre du plateau en renseignant séparément la ligne puis la colonne.

## 3. Interface console

- À chaque fois qu’une position est demandée, le jeu demande d’abord la ligne puis la colonne : pour chaque saisie, on peut indiquer un libellé alphabétique (`A`, `B`, …, `AA`) ou un numéro (`1`, `2`, …). Exemple : entrer `c` puis `7` pour la ligne C, colonne 7.
- Les menus affichent des options numérotées ; saisir le numéro voulu et valider.
- À chaque action importante, l’écran est nettoyé pour montrer l’état courant du plateau, des joueurs et des tuiles restantes.

## 4. Tour de jeu

1. **Choisir la source de tuile** :
   - piocher la prochaine tuile de la file ;
   - utiliser un coupon d’échange pour choisir l’une des cinq tuiles suivantes ;
   - dépenser un coupon pour retirer une pierre du plateau.
2. **Transformer la tuile** : rotations (90°/180°/270°) et miroirs horizontal/vertical sont disponibles autant de fois que souhaité.
3. **Prévisualiser le placement** : choisir une case d’ancrage ; la console affiche la superposition colorée.
4. **Valider ou annuler** :
   - en cas de validation, la tuile est posée, le territoire est mis à jour et les bonus capturés sont appliqués ;
   - sinon, revenir aux étapes précédentes ou défausser la tuile (elle est retirée définitivement du jeu).

## 5. Bonus et ressources

- Chaque joueur commence avec un coupon d’échange.
- Trois types de bonus existent :
  - **Échange (E)** : +1 coupon d’échange ;
  - **Pierre (P)** : obligation de poser immédiatement une pierre bloquante sur une case libre ;
  - **Vol (R)** : choix d’un adversaire pour lui voler une case (territoire et score).
- Un bonus est capturé lorsque les quatre cases orthogonales qui l’entourent appartiennent à la joueuse ou au joueur.
- Si une tuile recouvre directement un bonus, celui-ci est perdu sans effet.

## 6. Phase finale et condition de victoire

Lorsque la file de tuiles est vide ou qu’au moins neuf tours ont été joués :

- Chaque participant peut dépenser ses coupons restants pour acheter des tuiles 1×1 et les poser, en respectant les mêmes règles de placement.
- Le plus grand carré entièrement contrôlé départage le classement.
- En cas d’égalité sur la taille du carré maximal, le nombre total de cases contrôlées sert de second critère.

## 7. Conseils pratiques

- Garder un coupon pour la phase finale peut sécuriser une connexion clé.
- Utiliser la prévisualisation aussi souvent que nécessaire : aucune rotation ou transformation n’est définitive tant que la tuile n’est pas posée.
- Les pierres sont efficaces pour couper l’expansion adverse mais consomment des coupons ; planifier leur utilisation avant de capturer un bonus `P`.
- Surveiller la progression des autres couleurs dans l’interface : la légende du plateau rappelle les symboles importants (`#`, `.`, `S`, `E`, `P`, `R`).



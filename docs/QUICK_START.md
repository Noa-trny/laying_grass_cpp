# Guide Utilisateur (CLI)

## 1. Installation et Lancement

```bash
# Génération via CMake
mkdir build && cd build
cmake ..
cmake --build .

# ou directement avec le Makefile racine
make

# Exécution
./laying_grass
```

Le binaire s’appelle `laying_grass`. Assurez-vous que `data/tiles.json` est placé à la racine du projet (il est chargé automatiquement).

## 2. Préparation de la partie

1. **Nombre de joueurs** : le programme demande un entier entre 2 et 9 (inclus).
2. **Nom** : chaque joueur peut saisir un nom personnalisé (laisser vide utilise “Joueur X”).
3. **Couleur** : une liste de couleurs ANSI disponibles est proposée; choisissez un numéro libre.
4. **Ordre de jeu** : une fois toutes les saisies faites, l’ordre est mélangé et affiché.

## 3. Placement initial

- Tour à tour, chaque joueur choisit la case où poser sa tuile de départ 1×1.
- Saisir la ligne et la colonne avec des lettres (`A`, `B`, …, `AA`) ou des nombres (1…size).
- La case doit être libre (ni pierre ni bonus occupé).

## 4. Tour de jeu

Chaque tour suit ces étapes :

1. **Consultation des tuiles restantes** : le jeu affiche la file et vos coupons.
2. **Choix du tirage** :
   - Prendre la prochaine tuile.
   - Échanger contre l’une des cinq prochaines (coûte 1 coupon).
   - Retirer une pierre du plateau (coûte 1 coupon).
3. **Manipulation de la tuile** :
   - Rotation (0/90/180/270).
   - Symétrie horizontale ou verticale.
   - Visualisation overlay sur le plateau.
4. **Placement** :
   - Saisie de la position cible.
   - Confirmation obligatoire.
   - Si placement impossible, possibilité d’annuler (la tuile est défaussée).

## 5. Règles de pose appliquées automatiquement

- La première tuile doit être adjacente à la tuile de départ.
- Les tuiles suivantes doivent être adjacentes au territoire du joueur.
- Interdiction de chevaucher une autre tuile ou une pierre.
- Interdiction de toucher orthogonalement le territoire d’un adversaire.
- Les pierres bloquent les cases tant qu’elles ne sont pas retirées.

## 6. Gestion des bonus

Les cases bonus sont cachées sur la grille et se déclenchent lorsqu’un joueur couvre les quatre directions cardinales autour d’une case bonus :

- **Échange** : +1 coupon supplémentaire.
- **Pierre** : poser immédiatement une pierre sur une case vide.
- **Vol** : choisir un adversaire et transférer l’une de ses cases d’herbe.

Les effets sont appliqués instantanément avec affichage d’instructions.

## 7. Fin de partie

- Nombre maximum de tours : 9 poses par joueur.
- Phase finale : chaque coupon restant peut acheter une tuile 1×1 supplémentaire.
- Classement :
  1. Taille du plus grand carré de territoire.
  2. En cas d’égalité : nombre total de cases possédées.
- Résumé final : affichage des scores et du gagnant (ou des ex æquo).

## 8. Commandes utiles

| Action | Saisie |
|--------|--------|
| Confirmer / annuler | `o` / `n` |
| Choix menu | Numéro affiché |
| Coordonnées | Lettres (`A`, `B`, …) ou nombres (`1`, `2`, …) |
| Orientation | `0`, `90`, `180`, `270` |

## 9. Conseils d’utilisation

- Préférer des noms courts (affichage CLI).
- Utiliser les nombres pour les coordonnées sur les grandes grilles (30×30).
- En cas d’erreur de saisie, le jeu redemande automatiquement l’entrée.

Bon jeu ! 🟩

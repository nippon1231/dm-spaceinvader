# Galaga pour Mega Drive / Genesis

Un clone de Galaga développé pour la Sega Mega Drive/Genesis en utilisant SGDK (Sega Genesis Development Kit).

## Caractéristiques

- Vaisseau spatial contrôlable avec les directions gauche/droite
- Tir avec le bouton A
- Ennemis avec IA basique (mouvement en formation)
- Ennemis qui tirent sur le joueur
- Système de vagues
- Détection de collisions
- Système de score
- Game Over et redémarrage

## Structure du projet

```
galaga-megadrive/
├── src/              # Code source C
│   ├── main.c       # Point d'entrée principal
│   ├── game.c       # Logique de jeu principale
│   ├── player.c     # Gestion du joueur
│   ├── enemies.c    # Gestion des ennemis
│   └── bullets.c    # Gestion des projectiles
├── inc/              # Fichiers d'en-tête
│   └── game.h       # Déclarations principales
├── res/              # Ressources
│   ├── sprite/      # Sprites PNG
│   │   ├── player.png
│   │   ├── enemy.png
│   │   ├── bullet.png
│   │   └── explosion.png
│   ├── tileset/     # Tilesets
│   │   └── stars.png
│   └── resources.res # Fichier de définition des ressources SGDK
└── Makefile         # Makefile de compilation
```

## Prérequis

1. **SGDK (Sega Genesis Development Kit)**
   - Télécharger depuis: https://github.com/Stephane-D/SGDK
   - Installer et configurer la variable d'environnement GDK

2. **Émulateur Mega Drive**
   - Gens/GS (recommandé pour le développement)
   - Blastem
   - Fusion
   - RetroArch avec core Genesis Plus GX

## Compilation

### Sur Windows (recommandé pour débutants)

1. Assurez-vous que SGDK est installé et que la variable GDK pointe vers le répertoire SGDK

2. Double-cliquez sur `build.bat`

3. La ROM sera générée : `rom.bin`

### Avec Make (Linux/Mac ou Windows avec Make)

1. Dans le répertoire du projet, exécutez :
   ```bash
   make
   ```

2. Le fichier ROM sera généré : `out.bin`

### En cas d'erreur

Si vous obtenez une erreur "undefined reference to sprites", c'est que rescomp n'a pas été exécuté correctement. Solution :

1. Exécutez `clean.bat` (ou `make clean`)
2. Relancez la compilation

## Comment jouer

### Contrôles
- **Directionnel gauche/droite** : Déplacer le vaisseau
- **Bouton A** : Tirer
- **Start** : Redémarrer après Game Over

### Objectif
- Détruire tous les ennemis pour passer à la vague suivante
- Éviter les tirs ennemis
- Accumuler le score le plus élevé possible

## Personnalisation des sprites

Les sprites sont au format PNG et peuvent être modifiés avec n'importe quel éditeur d'image :

- **player.png** : 16x16 pixels - Le vaisseau du joueur
- **enemy.png** : 16x16 pixels - Les ennemis
- **bullet.png** : 8x8 pixels - Les projectiles
- **explosion.png** : 16x16 pixels - Animation d'explosion
- **stars.png** : 32x8 pixels - Fond étoilé (4 tuiles de 8x8)

**Important** : Respectez la palette de 16 couleurs maximum pour chaque sprite.

## Améliorations possibles

- [ ] Ajouter du son et de la musique
- [ ] Ajouter des animations d'explosion
- [ ] Différents types d'ennemis avec comportements variés
- [ ] Power-ups
- [ ] Boss de fin de vague
- [ ] Écran titre
- [ ] High score sauvegardé en SRAM
- [ ] Patterns de mouvement des ennemis plus complexes
- [ ] Plusieurs types de tirs

## Ressources utiles

- Documentation SGDK : https://github.com/Stephane-D/SGDK/wiki
- Tutoriels SGDK : https://www.ohsat.com/tutorial/
- Spécifications Mega Drive : http://md.squee.co/

## License

Ce projet est un exemple éducatif. Les sprites et le code sont libres d'utilisation.

## Crédits

Développé comme exemple de jeu Galaga-like pour Mega Drive avec SGDK.

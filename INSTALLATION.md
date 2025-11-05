# Guide d'installation - Galaga Mega Drive

## Prérequis

### 1. SGDK (Sega Genesis Development Kit)

**Téléchargement :**
https://github.com/Stephane-D/SGDK/releases

**Installation :**
1. Téléchargez la dernière version (ex: SGDK 2.00)
2. Extrayez dans `C:\sgdk` (ou un autre dossier sans espaces)
3. Créez la variable d'environnement `GDK`

**Créer la variable GDK :**
1. Clic droit sur "Ce PC" → Propriétés
2. Paramètres système avancés
3. Variables d'environnement
4. Nouvelle variable système :
   - Nom : `GDK`
   - Valeur : `C:\sgdk` (votre chemin SGDK)
5. OK → OK → OK
6. Redémarrez l'invite de commande

**Vérifier :**
```batch
echo %GDK%
```
Devrait afficher : `C:\sgdk`

### 2. Java Runtime Environment (JRE)

**Téléchargement :**
https://www.java.com/fr/download/

**Pourquoi Java ?**
SGDK utilise `rescomp.jar` pour compiler les ressources (sprites, musiques, etc.)

**Installation :**
1. Téléchargez et installez Java
2. Suivez l'assistant d'installation
3. Java sera ajouté automatiquement au PATH

**Vérifier :**
```batch
java -version
```
Devrait afficher la version de Java

## Compilation du projet

### Étape 1 : Extraire le projet
Décompressez `galaga-megadrive-simple.zip` dans un dossier, par exemple :
```
C:\git\galaga-megadrive\
```

### Étape 2 : Compiler
Double-cliquez sur `build.bat`

OU en ligne de commande :
```batch
cd C:\git\galaga-megadrive
build.bat
```

### Étape 3 : Tester
Ouvrez `galaga.bin` avec un émulateur :
- Gens/GS (recommandé)
- Blastem
- Fusion
- RetroArch

## Résolution des problèmes

### "GDK non définie"
→ La variable d'environnement GDK n'est pas configurée
→ Suivez les étapes ci-dessus pour créer la variable

### "Java n'est pas installé"
→ Installez Java depuis https://www.java.com
→ Redémarrez l'invite de commande après installation

### "rescomp a échoué"
→ Vérifiez que vos sprites PNG sont corrects (128x32 minimum)
→ Vérifiez que Java est installé

### "make.exe introuvable"
→ Vérifiez que SGDK est bien installé
→ Vérifiez que %GDK%\bin\make.exe existe

### La ROM ne démarre pas
→ Essayez différents émulateurs
→ Vérifiez que la ROM fait plus de 0 octets

## Émulateurs recommandés

**Gens/GS** (le plus simple)
- http://www.gens.me/
- Glisser-déposer la ROM

**Blastem** (le plus précis)
- https://www.retrodev.com/blastem/
- Excellent pour le développement

**RetroArch** (multi-systèmes)
- https://www.retroarch.com/
- Core: Genesis Plus GX

## Structure du projet

```
galaga-megadrive/
├── src/           # Code source C
│   └── main.c
├── res/           # Ressources
│   ├── resources.res
│   └── player.png
├── build.bat      # Script de compilation
└── README.md
```

## Ressources utiles

- SGDK Wiki : https://github.com/Stephane-D/SGDK/wiki
- Tutoriels : https://www.ohsat.com/tutorial/
- Forum : https://segaxtreme.net/forums/

## Besoin d'aide ?

Si vous rencontrez des problèmes :
1. Vérifiez que SGDK et Java sont installés
2. Vérifiez les variables d'environnement
3. Consultez la documentation SGDK
4. Demandez sur les forums de développement Mega Drive

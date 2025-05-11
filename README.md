# 🛡️ DefendTheBackrooms

**DefendTheBackrooms** est un jeu *Tower Defense* original se déroulant dans l'univers mystérieux des Backrooms (OU PAS). Placez vos tourelles (singes), gérez vos ressources, repoussez les vagues d'ennemis (crabes), et survivez le plus longtemps possible. Le jeu utilise la bibliothèque SDL2 et plusieurs de ses extensions pour proposer une expérience immersive et dynamique.

---

## 📦 Fonctionnalités

- 🗺️ Carte 2D générée procéduralement avec chemin valide pour les ennemis.
- 🧠 Pathfinder intégré pour le déplacement des ennemis.
- 🗼 Placement de tourelles et modes "placement" / "upgrade".
- 💰 Système d'économie en jeu.
- 🎵 Musiques et sons intégrés via `SDL_mixer`.
- 🖼️ Interface utilisateur via système de GUI personnalisée.
- 💾 Système de sauvegarde/chargement.
- 🔀 Interface animée avec menus, boutons, transitions.

---

## 🧱 Dépendances

Le projet repose sur **SDL2** et plusieurs de ses surcouches :

- `SDL2`
- `SDL2_image`
- `SDL2_ttf`
- `SDL2_mixer`
- `libm` (math)

### Installation des dépendances sous Linux (Debian/Ubuntu)

```bash
sudo apt update
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev

####Compilation

compiler :
```bash
make


lancement du jeu :
```bash
make run


nettoyage :
```bash
make clean


#####APPRECIEZ LE JEU!!!!


# 🖼️ Système d'Interface Graphique (GUI) simple avec SDL

Ce framework léger utilise **SDL2**, **SDL_ttf** et **SDL_image** pour créer facilement des **widgets interactifs** : textes, boîtes colorées et bientôt des images.

---

## 📦 Fonctionnalités

- ✅ Ajout facile dans n’importe quel projet SDL
- 🧱 Widgets disponibles : Texte, Boîte, Collider, Image
- 🎯 Gestion du survol et des clics
- 🔧 Possibilité d'ajouter des comportements personnalisés
- 🏗️ Léger, modulaire et extensible

---

## 🚀 Démarrage rapide

### 1. Initialisation de SDL

Vous aurez besoin d’un `SDL_Renderer` et éventuellement d’une police (`TTF_Font`) pour les widgets texte :

```c
TTF_Font *font = TTF_OpenFont("assets/font.TTF", 24);
if (!font) {
    fprintf(stderr, "Erreur chargement de la police : %s\n", TTF_GetError());
    return 1;
}
```

---

### 2. Création du GUI

Un `Gui` contient tous vos widgets :

```c
Gui gui = {.widget_count = 0};
```

---

### 3. Ajout de widgets

#### 🟥 Boîte colorée (Box)

```c
Box *box = make_box_widget(
    (SDL_Rect){100, 50, 100, 40},
    (SDL_Color){200, 50, 100, 255},
    true,
    change_color_on_hover
);
gui.widgets[gui.widget_count++] = (Widget *)box;
```

#### 🔤 Texte

```c
Text *label = make_text_widget(
    (SDL_Rect){50, 150, 0, 0},
    "Clique-moi !",
    (SDL_Color){255, 255, 255, 255},
    font,
    change_size_on_click
);
gui.widgets[gui.widget_count++] = (Widget *)label;
```

#### ✋ Collider (pour interactivité)

```c
Collider *collider = create_collider_for((Widget *)label);
gui.widgets[gui.widget_count++] = (Widget *)collider;
```

---

### 4. Boucle principale

Appelez ces fonctions à chaque frame :

```c
interact_gui(&gui);                      // Détection clic/survol
update_gui(&gui, &context);              // Met à jour les états
draw_gui_visible_components(&gui, renderer);  // Affiche les widgets
```

Et une seule fois au début :

```c
gui_init(&gui);  // Sauvegarde les couleurs et dimensions originales
```

---

## 🧠 Comportements personnalisés

Chaque widget peut avoir une fonction de comportement :

```c
void change_color_on_hover(Widget *self, const Context *context) {
    if (*self->selected) {
        self->color = *self->clicked
            ? (SDL_Color){100, 100, 50, 255}
            : (SDL_Color){170, 170, 50, 255};
    } else {
        self->color = self->default_color;
    }
}
```

---

## 🖼️ Widget image (expérimental)

Affichage simple d’image :

```c
Image *img = malloc(sizeof(Image));
img->widget.rect = (SDL_Rect){300, 100, 64, 64};
img->widget.type = WIDGET_IMAGE;
img->texture = IMG_LoadTexture(renderer, "assets/image.png");
gui.widgets[gui.widget_count++] = (Widget *)img;
```

---

## 📊 Types de widgets

| Type       | Structure   | Besoin police ? | Interactif ? | Fonction d'affichage   |
|------------|-------------|------------------|--------------|-------------------------|
| Texte      | `Text`      | ✅ Oui            | Optionnel    | `draw_text`             |
| Boîte      | `Box`       | ❌ Non            | Optionnel    | `draw_box`              |
| Image      | `Image`     | ❌ Non            | ❌ Non       | `draw_image`            |
| Collider   | `Collider`  | ❌ Non            | ✅ Oui       | (gère clic/survol)      |

---

## 🛠️ Fonctions utilitaires

- `mouse_in_rect(SDL_Rect *)` → Vérifie si la souris est dans un rectangle
- `mousedown()` → Vérifie si le bouton gauche est cliqué
- `create_collider_for(Widget *)` → Ajoute un collider automatiquement autour d’un widget

---

## 📚 Exemple complet

```c
Gui gui = {.widget_count = 0};

Box *box = make_box_widget((SDL_Rect){100, 100, 120, 60}, (SDL_Color){0, 120, 200, 255}, true, change_color_on_hover);
gui.widgets[gui.widget_count++] = (Widget *)box;

Collider *col = create_collider_for((Widget *)box);
gui.widgets[gui.widget_count++] = (Widget *)col;

gui_init(&gui);
```

Dans la boucle de rendu :

```c
interact_gui(&gui);
update_gui(&gui, &context);
draw_gui_visible_components(&gui, renderer);
```

---

## 🧪 Fonctionnalités à venir

- ✅ Chargement d'images amélioré
- 🔘 Boutons cliquables
- 🔡 Champs de texte
- 🔲 Outils de layout (alignement, espacement)
- 📣 Système d’événements (`onClick`, `onHover`, etc.)

---

## ❤️ Contribution

N’hésitez pas à **forker**, modifier ou proposer des améliorations. Ce framework vise à rester **simple, lisible et modulaire**.

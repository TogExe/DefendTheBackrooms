
# 🧱 Framework GUI Simple pour SDL2

Ce framework léger en C permet de créer facilement des **interfaces graphiques interactives** avec **SDL2**, **SDL_ttf** et **SDL_image**.  
Il propose des composants de base comme **du texte**, **des boîtes colorées**, **des images**, ainsi que **des interactions (survol, clic)**.

### ⚠️ Une  mise a jour a été appliqué afin de s'assurer de bien maintenir la mémoire.

---

## 🔧 Fonctionnalités

- 🚀 Facile à intégrer dans n'importe quel projet SDL2
- 🧱 Widgets disponibles : `Texte`, `Boîte`, `Image`, `Collider`
- 🖱️ Détection du survol et du clic avec des colliders
- 🧠 **Procédures personnelles** pour comportements personnalisés
- 📦 Système de `Context` pour connecter l'interface à l'état du programme

---

## 📦 Installation de base

### 1. Initialiser SDL et TTF

Avant d'utiliser du texte, initialisez SDL_ttf et chargez une police :

```c
TTF_Font *font = TTF_OpenFont("assets/font.TTF", 24);
if (!font) {
    fprintf(stderr, "Erreur chargement police : %s\n", TTF_GetError());
    return 1;
}
```

---

### 2. Créer un conteneur GUI

```c
Gui gui = {.widget_count = 0};
gui.w = WIDTH // a remplacer par un int positif
gui.h = HEIGHT// a remplacer par un int positif
```

---

### 3. Ajouter des widgets

#### 🟦 Boîte

```c
Box *box = make_box_widget(
    (SDL_Rect){100, 50, 120, 60},
    (SDL_Color){0, 120, 200, 255},
    true,
    change_color_on_hover
);
gui.widgets[gui.widget_count++] = (Widget *)box;
```

#### 🔤 Texte

```c
Text *label = make_text_widget(
    (SDL_Rect){80, 140, 0, 0},
    "Cliquez-moi !",
    (SDL_Color){255, 255, 255, 255},
    font,
    change_size_on_click
);
gui.widgets[gui.widget_count++] = (Widget *)label;
```

#### 🖱️ Collider

Pour rendre un widget interactif :

```c
Collider *collider = create_collider_for((Widget *)label);
gui.widgets[gui.widget_count++] = (Widget *)collider;
```

---

## 🔁 Boucle principale

Dans votre boucle principale SDL :

```c
interact_gui(&gui);                       // Gère le survol / clic
update_gui(&gui, &context);              // Applique les comportements
draw_gui_visible_components(&gui, renderer);  // Affiche les widgets
```

Une fois au démarrage :

```c
gui_init(&gui);  // Enregistre les couleurs/positions de base
```

---

## 🧠 Procédures personnelles & Contexte

Une **procédure personnelle** est une fonction qui définit un comportement spécial pour un widget.

Exemple : changer la couleur au survol ou au clic :

```c
void change_color_on_hover(Widget *self, const Context *context) {
    if (*self->selected) {
        if (*self->clicked) {
            self->color = (SDL_Color){100, 100, 50, 255};
        } else {
            self->color = (SDL_Color){170, 170, 50, 255};
        }
    } else {
        self->color = self->default_color;
    }
}
```

Vous pouvez également utiliser le `Context` pour modifier l'état global :

```c
typedef struct Context {
    int *score;
    bool *game_over;
} Context;

void update_score_on_click(Widget *self, const Context *context) {
    if (*self->clicked && context && context->score) {
        *(context->score) += 1;
        printf("Score : %d\n", *(context->score));
    }
}
```

Et dans `main` :

```c
int score = 0;
Context ctx = {.score = &score};

update_gui(&gui, &ctx);  // Transmet l'état de l'appli à l'interface
```

---

## 🎨 Affichage

- `draw_gui_visible_components()` affiche automatiquement :
  - `Text` avec `draw_text()`
  - `Box` avec `draw_box()`
  - `Image` avec `draw_image()`

---

## 📚 Exemple

```c
Gui gui = {.widget_count = 0};

Box *button = make_box_widget((SDL_Rect){100, 100, 120, 60}, (SDL_Color){100, 50, 200, 255}, true, change_color_on_hover);
gui.widgets[gui.widget_count++] = (Widget *)button;
gui.widgets[gui.widget_count++] = (Widget *)create_collider_for((Widget *)button);

Text *label = make_text_widget((SDL_Rect){110, 110, 0, 0}, "Appuyez", (SDL_Color){255, 255, 255, 255}, font, update_score_on_click);
gui.widgets[gui.widget_count++] = (Widget *)label;
gui.widgets[gui.widget_count++] = (Widget *)create_collider_for((Widget *)label);

int score = 0;
Context context = {.score = &score};

gui_init(&gui);

while (running) {
    interact_gui(&gui);
    update_gui(&gui, &context);
    draw_gui_visible_components(&gui, renderer);
}
free_gui(&gui) // pour libérer la mémoire
```

---

## 🛠 Fonctions Utiles *(utilisées par les widgets)*

| Fonction                | Description                                  |
|-------------------------|----------------------------------------------|
| `mousedown()`           | Vérifie si le clic gauche est actif          |
| `mouse_in_rect(rect*)`  | Vérifie si la souris est dans un rectangle   |
| `create_collider_for()` | Crée un collider autour d’un widget          |

---

## 🧩 Référence des Widgets

| Type     | Struct     | Police ? | Interactif ? | Visible ? |
|----------|------------|----------|--------------|-----------|
| Texte    | `Text`     | ✅ Oui    | ✅ Oui        | ✅ Oui     |
| Boîte    | `Box`      | ❌ Non    | ✅ Oui        | ✅ Oui     |
| Image    | `Image`    | ❌ Non    | ❌ Non        | ✅ Oui     |
| Collider | `Collider` | ❌ Non    | ✅ Oui        | ❌ Non     |

---

## 📅 Feuille de Route

- [x] Textes et boîtes de base
- [x] Survol et clic
- [x] Procédures personnalisées
- [ ] Boutons image avec redimensionnement
- [ ] Champs de texte
- [ ] Boutons radio / toggles
- [ ] Aides au placement automatique (grid/layout)
- [ ] Événements (`onClick`, `onEnter`, etc.)

---

*merci de la lecture et bonne chance :)*

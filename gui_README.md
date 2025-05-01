# 🖼️ Simple SDL GUI Framework

This is a lightweight GUI system built with **SDL2**, **SDL_ttf**, and **SDL_image** designed to let you easily create **interactive widgets** like text, boxes, and (soon) images in your C applications.

---

## 📦 Features

- ✅ Easy setup and integration with SDL
- 🧱 Widgets: Boxes, Text, Colliders, Images
- 🎯 Hover and click interactivity
- 🔧 Custom behavior using widget callbacks
- 🏗️ Built to be extended and lightweight

---

## 🚀 Getting Started

### 1. Set up SDL

You need an `SDL_Renderer` and optionally a `TTF_Font` for text widgets:

```c
TTF_Font *font = TTF_OpenFont("assets/font.TTF", 24);
if (!font) {
    fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
    return 1;
}
```

---

### 2. Create the GUI

A `Gui` holds all your widgets:

```c
Gui gui = {.widget_count = 0};
```

---

### 3. Add Widgets

#### 🟥 Box Widget

```c
Box *box = make_box_widget(
    (SDL_Rect){100, 50, 100, 40},
    (SDL_Color){200, 50, 100, 255},
    true,
    change_color_on_hover
);
gui.widgets[gui.widget_count++] = (Widget *)box;
```

#### 🔤 Text Widget

```c
Text *label = make_text_widget(
    (SDL_Rect){50, 150, 0, 0},
    "Click Me!",
    (SDL_Color){255, 255, 255, 255},
    font,
    change_size_on_click
);
gui.widgets[gui.widget_count++] = (Widget *)label;
```

#### ✋ Collider Widget

Colliders enable interaction (hovering and clicking):

```c
Collider *collider = create_collider_for((Widget *)label);
gui.widgets[gui.widget_count++] = (Widget *)collider;
```

---

### 4. Main Loop Functions

In your main game loop, call these every frame:

```c
interact_gui(&gui);                  // Detect interaction
update_gui(&gui, &context);          // Update widget behavior
draw_gui_visible_components(&gui, renderer); // Render widgets
```

Call once at the beginning:

```c
gui_init(&gui);                      // Set origin/default colors
```

---

## 🧠 Custom Behavior

Each widget can have a custom function:

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

You can assign it when creating a widget.

---

## 🖼️ Image Widget (Preview)

Image support is being added:

```c
Image *img = malloc(sizeof(Image));
img->widget.rect = (SDL_Rect){300, 100, 64, 64};
img->widget.type = WIDGET_IMAGE;
img->texture = IMG_LoadTexture(renderer, "assets/icon.png");
gui.widgets[gui.widget_count++] = (Widget *)img;
```

---

## 🧩 Widget Types

| Type      | Struct   | Needs Font | Interactive | Rendered By         |
|-----------|----------|------------|-------------|----------------------|
| Box       | `Box`    | No         | Optional    | `draw_box`           |
| Text      | `Text`   | Yes        | Optional    | `draw_text`          |
| Image     | `Image`  | No         | No          | `draw_image`         |
| Collider  | `Collider` | No       | Yes         | (Logic only)         |

---

## 🧰 Utility Functions

- `mouse_in_rect(SDL_Rect *)` - Check if mouse is inside a rect
- `mousedown()` - Check if left mouse button is held
- `create_collider_for(Widget *)` - Automatically wraps a widget with a collider

---

## 📌 Notes

- Call `SDL_Init`, `TTF_Init`, and `IMG_Init` before using this system.
- Always clean up textures and fonts you load.
- Currently supports basic layout and hover/click logic; no scrolling, resizing, or input fields yet.

---

## 📚 Example

Here’s a minimal example showing how to set up a box and make it interactive:

```c
Gui gui = {.widget_count = 0};
Box *box = make_box_widget((SDL_Rect){100, 100, 120, 60}, (SDL_Color){0, 120, 200, 255}, true, change_color_on_hover);
gui.widgets[gui.widget_count++] = (Widget *)box;

Collider *col = create_collider_for((Widget *)box);
gui.widgets[gui.widget_count++] = (Widget *)col;

gui_init(&gui);
```

In your game loop:

```c
interact_gui(&gui);
update_gui(&gui, &context);
draw_gui_visible_components(&gui, renderer);
```

---

## 🧪 Coming Soon

- ✅ Image loading improvements
- 🔲 Button widgets
- 🔢 Input fields
- 🎨 Layout helpers (auto-aligning, spacing)
- 🧩 Event system (onClick, onHover, etc.)

---

## ❤️ Contributing

Feel free to fork, use, and suggest improvements. This is meant to stay minimal, readable, and modular.

---

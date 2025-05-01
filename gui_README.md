
# 🧱 Simple GUI Framework for SDL2

This is a lightweight C framework to create **interactive GUI widgets** using **SDL2**, **SDL_ttf**, and **SDL_image**.  
It supports basic interface components like **text**, **colored boxes**, **images**, and **interactivity (hover/click)**.

---

## 🔧 Features

- 🚀 Plug-and-play integration with any SDL2 project
- 🧱 Ready-to-use widgets: `Text`, `Box`, `Image`, `Collider`
- 🖱️ Hover & click detection via colliders
- 🎯 Custom behaviors via **personal procedures**
- 🧠 Shared `Context` object for global state interaction

---

## 📦 Basic Setup

### 1. Initialize SDL & TTF

For text widgets, initialize SDL_ttf and load a font:

```c
TTF_Font *font = TTF_OpenFont("assets/font.TTF", 24);
if (!font) {
    fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
    return 1;
}
```

---

### 2. Create a GUI container

The `Gui` struct holds all widgets:

```c
Gui gui = {.widget_count = 0};
```

---

### 3. Add Widgets

#### 🟦 Box Widget

```c
Box *box = make_box_widget(
    (SDL_Rect){100, 50, 120, 60},
    (SDL_Color){0, 120, 200, 255},
    true,
    change_color_on_hover  // personal procedure
);
gui.widgets[gui.widget_count++] = (Widget *)box;
```

#### 🔤 Text Widget

```c
Text *label = make_text_widget(
    (SDL_Rect){80, 140, 0, 0},
    "Click me!",
    (SDL_Color){255, 255, 255, 255},
    font,
    change_size_on_click
);
gui.widgets[gui.widget_count++] = (Widget *)label;
```

#### 🖱️ Collider Widget

Attach interactivity to any widget using a collider:

```c
Collider *collider = create_collider_for((Widget *)label);
gui.widgets[gui.widget_count++] = (Widget *)collider;
```

---

## 🔁 Main Loop Integration

In your main render/update loop:

```c
interact_gui(&gui);                       // Update hover/click state
update_gui(&gui, &context);              // Run personal procedures
draw_gui_visible_components(&gui, renderer);  // Draw GUI
```

Once at the beginning:

```c
gui_init(&gui);  // Stores original colors and positions
```

---

## 🧠 Personal Procedures & Context

A **personal procedure** is a custom function tied to a widget, run on click/interaction.

Example: Changing color on hover and click:

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

You can also use the `Context` to interact with your app’s global state:

```c
typedef struct Context {
    int *score;
    bool *game_over;
    // Any other global state...
} Context;

void update_score_on_click(Widget *self, const Context *context) {
    if (*self->clicked && context && context->score) {
        *(context->score) += 1;
        printf("Score: %d\n", *(context->score));
    }
}
```

Then in `main`:

```c
int score = 0;
Context ctx = {.score = &score};

update_gui(&gui, &ctx);  // Pass your app context every frame
```

---

## 🎨 Drawing

- `draw_gui_visible_components()` automatically dispatches drawing for:
  - `Text`: `draw_text()`
  - `Box`: `draw_box()`
  - `Image`: `draw_image()`

---

## 📚 Example

```c
Gui gui = {.widget_count = 0};

Box *button = make_box_widget((SDL_Rect){100, 100, 120, 60}, (SDL_Color){100, 50, 200, 255}, true, change_color_on_hover);
gui.widgets[gui.widget_count++] = (Widget *)button;
gui.widgets[gui.widget_count++] = (Widget *)create_collider_for((Widget *)button);

Text *label = make_text_widget((SDL_Rect){110, 110, 0, 0}, "Press", (SDL_Color){255, 255, 255, 255}, font, update_score_on_click);
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
```

---

## 🛠 Utility Functions

| Function                | Purpose                                 |
|-------------------------|-----------------------------------------|
| `mousedown()`           | Returns true if left mouse is down      |
| `mouse_in_rect(rect*)`  | Checks if mouse is inside a rectangle   |
| `create_collider_for()` | Creates collider around a widget        |

---

## 🧩 Widget Reference

| Type     | Struct     | Needs font? | Interactive? | Drawable? |
|----------|------------|-------------|--------------|-----------|
| Text     | `Text`     | ✅ Yes       | ✅ Yes        | ✅ Yes     |
| Box      | `Box`      | ❌ No        | ✅ Yes        | ✅ Yes     |
| Image    | `Image`    | ❌ No        | ❌ No         | ✅ Yes     |
| Collider | `Collider` | ❌ No        | ✅ Yes        | ❌ No      |

---

## 📅 Roadmap (Planned)

- [x] Text and box widgets
- [x] Hover/click detection
- [x] Personal behavior procedures
- [ ] Image buttons with scaling
- [ ] Input text fields
- [ ] Radio buttons and toggles
- [ ] Grid/layout helpers
- [ ] Event hooks (`onClick`, `onEnter`, etc.)

---

*thanks for reading*

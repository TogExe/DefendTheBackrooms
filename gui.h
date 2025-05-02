#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

// Define maximum widgets for GUI
#define MAX_WIDGETS 128
#define MAX_WIDGETS 128


// Widget types
typedef enum {
    WIDGET_TEXT,
    WIDGET_BOX,
    WIDGET_COLLIDER,
    WIDGET_IMAGE,
    WIDGET_ROOT
} WidgetType;

typedef struct Context Context;
typedef struct Widget Widget;

bool mouse_in_rect(const SDL_Rect* rect);
bool mousedown();

// Base widget
typedef struct Widget {
    SDL_Rect origin;
    SDL_Rect rect;
    SDL_Color color;
    SDL_Color default_color;
    void (*personal_procedure)(Widget *self,Context * context);
    bool *clicked;
    bool *selected;
    WidgetType type;
    void * upper_component;
    void (*deletion_procedure)(void*);
} Widget;

typedef struct Complement {
    Widget widget;
} Complement;

// Text widget
typedef struct Text {
    Widget widget;
    char *content;
    SDL_Color color;
    TTF_Font *font;
    float size_multiplier;
} Text;
// the free procedure

// Box widget
typedef struct Box {
    Widget widget;
    bool is_visible;
    SDL_Rect bounds;
    SDL_Color color;
} Box;
// to free a box

// Collider widget
typedef struct Collider {
    Widget widget;
    Widget *target_widget;
    bool interacted_with;
    bool hover;
} Collider;
// to free a collider

// Image widget
typedef struct Image {
    Widget widget;
    float scale_multiplier;
    SDL_Texture *texture;
} Image;
// to free an image

// GUI structure
typedef struct Gui {
    Widget *widgets[MAX_WIDGETS];
    int widget_count;
    int w;
    int h;
} Gui;

// Widget types

// === Initialization === //
void gui_init(const Gui *gui);
void free_gui(const Gui *gui);
void bind_gui(const Gui *gui);

// === Drawing functions === //
void draw_text(SDL_Renderer *renderer, Text *text);
void draw_box(SDL_Renderer *renderer, Box *box);
void draw_image(SDL_Renderer *renderer, Image *image);
void draw_gui_visible_components(const Gui *gui, SDL_Renderer *renderer);

// === Interaction and update functions === //
void interact_gui(const Gui *gui);
void update_gui(const Gui *gui, Context * context);

// === Free system ===
void free_box(void * object);
void free_text(void * object);
void free_image(void * object);
void free_collider(void * object);

// === Widget binding back system ===
bool if_collider_bind(void * object);
bool if_text_bind(void * object);
bool if_box_bind(void * object);

// === Widget creation functions === //
Text* make_text_widget(const SDL_Rect rect, const char *content, const SDL_Color color, TTF_Font *font, void (*personal_procedure)(Widget *self,Context * context));
Box * make_box_widget(const SDL_Rect rect, const SDL_Color color,const bool visible, void (*personal_procedure)(Widget *self, Context * context));
Collider* make_collider_widget(const SDL_Rect rect, Widget *target_widget);
Collider* create_collider_for(Widget *target_widget);

// === Utility functions === //
bool mouse_in_rect(const SDL_Rect *rect);
bool mousedown();

// === Root widget procedures === //
void change_color_on_hover(Widget *self, Context * context);
void change_size_on_click(Widget *self, Context * context);
void exit_on_click(Widget *self, Context * context);

#endif // GUI_H

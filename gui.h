#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

// Define maximum widgets for GUI
#define MAX_WIDGETS 128

// Widget types
typedef enum {
    WIDGET_TEXT,
    WIDGET_BOX,
    WIDGET_COLLIDER,
    WIDGET_IMAGE,
    WIDGET_ROOT
} WidgetType;

// Forward declaration of Context and Widget structures
typedef struct Context Context;
typedef struct Widget Widget;

// Widget structure
typedef struct Widget {
    SDL_Rect origin;  // The original position and size
    SDL_Rect rect;    // The current position and size
    SDL_Color color;  // The color of the widget
    SDL_Color default_color; // Default color
    void (*personal_procedure)(Widget *self, const Context *context); // Custom procedure for the widget
    bool *clicked;     // Whether the widget was clicked
    bool *selected;    // Whether the widget is selected
    WidgetType type;   // The type of widget (text, box, etc.)
} Widget;

// Text widget structure
typedef struct Text {
    Widget widget;
    char *content;      // The text content
    SDL_Color color;    // The color of the text
    TTF_Font *font;     // Font for the text
    float size_multiplier; // Size multiplier for the text
} Text;

// Box widget structure
typedef struct Box {
    Widget widget;
    bool is_visible;    // Whether the box is visible
    SDL_Rect bounds;    // Bounds of the box
    SDL_Color color;    // Color of the box
} Box;

// Collider widget structure
typedef struct Collider {
    Widget widget;
    Widget *target_widget;   // The widget to which this collider is attached
    bool interacted_with;    // Whether the collider was interacted with
    bool hover;              // Whether the mouse is hovering over the collider
} Collider;

// Image widget structure
typedef struct Image {
    Widget widget;
    float scale_multiplier;  // Scale multiplier for the image
    SDL_Texture *texture;    // Texture for the image
} Image;

// GUI structure
typedef struct Gui {
    Widget *widgets[MAX_WIDGETS];  // Array of widgets
    int widget_count;// The number of widgets
    int w;// The screen width to set a corner 
    int h;// The screen height
} Gui;

// Function declarations

// === Initialization === //
void gui_init(const Gui *gui);

// === Drawing functions === //
void draw_text(SDL_Renderer *renderer, Text *text);
void draw_box(SDL_Renderer *renderer, Box *box);
void draw_image(SDL_Renderer *renderer, Image *image);
void draw_gui_visible_components(const Gui *gui, SDL_Renderer *renderer);

// === Interaction and update functions === //
void interact_gui(const Gui *gui);
void update_gui(const Gui *gui, const Context *context);

// === Widget creation functions === //
Text* make_text_widget(const SDL_Rect rect, const char *content, const SDL_Color color, TTF_Font *font, void (*personal_procedure)(Widget *self, const Context *context));
Box* make_box_widget(const SDL_Rect rect, const SDL_Color color, const bool visible, void (*personal_procedure)(Widget *self, const Context *context));
Collider* make_collider_widget(const SDL_Rect rect, Widget *target_widget);
Collider* create_collider_for(Widget *target_widget);

// === Utility functions === //
bool mouse_in_rect(const SDL_Rect *rect);
bool mousedown();

// === Root widget procedures === //
void change_color_on_hover(Widget *self, const Context *context);
void change_size_on_click(Widget *self, const Context *context);

#endif // GUI_H

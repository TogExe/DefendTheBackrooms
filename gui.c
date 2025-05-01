#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    void (*personal_procedure)(Widget *self,const Context * context);
    bool *clicked;
    bool *selected;
    WidgetType type;
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

// Box widget
typedef struct Box {
    Widget widget;
    bool is_visible;
    SDL_Rect bounds;
    SDL_Color color;
} Box;

// Collider widget
typedef struct Collider {
    Widget widget;
    Widget *target_widget;
    bool interacted_with;
    bool hover;
} Collider;

// Image widget
typedef struct Image {
    Widget widget;
    float scale_multiplier;
    SDL_Texture *texture;
} Image;

// GUI structure
typedef struct Gui {
    Widget *widgets[MAX_WIDGETS];
    int widget_count;
} Gui;

// === Drawing functions === //
void draw_text(SDL_Renderer *renderer, Text *text) {
    if (!text || !text->font || !text->content) return;
    SDL_Surface *surface = TTF_RenderText_Blended(text->font, text->content, text->widget.color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = text->widget.rect;
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void draw_box(SDL_Renderer *renderer, Box *box) {
    if (!box || !box->is_visible) return;
    SDL_SetRenderDrawColor(renderer, box->widget.color.r, box->widget.color.g, box->widget.color.b, box->widget.color.a);
    SDL_RenderFillRect(renderer, &box->widget.rect);
    printf("Drawing Box at (%d, %d) with size (%d, %d)\n", box->widget.rect.x, box->widget.rect.y, box->widget.rect.w, box->widget.rect.h);
}

void draw_image(SDL_Renderer *renderer, Image *image) {
    if (!image || !image->texture) return;
    const SDL_Rect dst = image->widget.rect;
    SDL_RenderCopy(renderer, image->texture, NULL, &dst);
}

void draw_gui_visible_components(const Gui *gui, SDL_Renderer *renderer){
    for (int i = 0; i < gui->widget_count; i++) {
        Widget *w = gui->widgets[i];
        if (!w) continue;

        /*if (w->personal_procedure)
            w->personal_procedure(w->clicked);*/

        switch (w->type) {
            case WIDGET_TEXT:
                draw_text(renderer, (Text *)w);
                break;
            case WIDGET_BOX:
                draw_box(renderer, (Box *)w);
                break;
            case WIDGET_IMAGE:
                draw_image(renderer, (Image *)w);
                break;
            default:
                break;
        }
    }
}

// === Initializing and closing ===
void gui_init(const Gui *gui)
{
    for (int i = 0; i < gui->widget_count; i++)
    {
        Widget *w = gui->widgets[i];
        if (!w) continue;
        w->origin = w->rect;
        w->default_color = w->color;
    }
}

// === Letting widgets actually do stuff ===

void interact_gui(const Gui *gui)
{
    const bool mouse_is_down = mousedown();
    for (int i = 0; i < gui->widget_count; i++) {
        Widget *w = gui->widgets[i];
        if (!w || w->type != WIDGET_COLLIDER) continue;

        Collider *collider = (Collider *)w;
        if (!collider->target_widget) continue;

        const bool in_rect = mouse_in_rect(&collider->widget.rect);
        printf("collider state : %d %d", mouse_is_down,in_rect);
        collider->hover = in_rect;
        collider->target_widget->selected = &collider->hover;
        collider->interacted_with = (in_rect && mouse_is_down);
        collider->target_widget->clicked = &collider->interacted_with;
    }
}

void update_gui(const Gui *gui,const Context * context)
{
    for (int i = 0; i < gui->widget_count; i++)
    {
        Widget *w = gui->widgets[i];
        if (!w) continue;
        if (w->personal_procedure)
        {
            if (w->clicked) w->personal_procedure(w,context);
        }
    }
}

// === Root widget procedures ===

void change_color_on_hover(Widget *self,const Context * context)
{
    if (*self->selected)
    {
        if ( *self->clicked)
        {
            printf("tex_clicked");
            self->color.r = 100;
            self->color.g = 100;
            self->color.b = 50;
            self->color.a = 255;
        }
        else
        {
            printf("tex_hover");
            self->color.r = 170;
            self->color.g = 170;
            self->color.b = 50;
            self->color.a = 255;
        }
    }
    else
    {
        self->color = self->default_color;
    }
}

void change_size_on_click(Widget *self,const Context * context)
{
    if (*self->clicked)
    {
        printf("Self got clicked yayy\n");
        self->rect.w = self->origin.w-20;
        self->rect.h = self->origin.h-20;
        self->rect.x = self->origin.x + (self->origin.w - self->rect.w)/2;
        self->rect.y = self->origin.y + (self->origin.h - self->rect.h)/2;
    }
    else
    {
        self->rect.w = self->origin.w;
        self->rect.h = self->origin.h;
        self->rect.x = self->origin.x;
        self->rect.y = self->origin.y;
    }
}

// === Utility ===

bool mouse_in_rect(const SDL_Rect * rect) {
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    return (mx >= rect->x && mx <= rect->x + rect->w &&
            my >= rect->y && my <= rect->y + rect->h);
}

bool mousedown() {
    const Uint32 mouseState = SDL_GetMouseState(NULL, NULL);  // Get the current mouse state
    return (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;  // Check if the left mouse button is down
}

// === Widget creation functions ===

Text* make_text_widget(const SDL_Rect rect, const char *content, const SDL_Color color, TTF_Font *font, void (*personal_procedure)(Widget *self,const Context * context)) {
    Text *text = malloc(sizeof(Text));
    text->widget.rect = rect;
    text->widget.personal_procedure = personal_procedure;
    text->widget.clicked = NULL;
    text->widget.type = WIDGET_TEXT;
    text->widget.color = color;
    text->content = strdup(content);
    text->font = font;
    text->size_multiplier = 1.0f;
    return text;
}

Box * make_box_widget(const SDL_Rect rect, const SDL_Color color,const bool visible, void (*personal_procedure)(Widget *self,const Context * context)) {
    Box *box = malloc(sizeof(Box));
    box->widget.rect = rect;
    box->widget.personal_procedure = personal_procedure;
    box->widget.clicked = NULL;
    box->is_visible = visible;
    box->widget.type = WIDGET_BOX;
    box->widget.color = color;
    return box;
}

Collider* make_collider_widget(const SDL_Rect rect, Widget *target_widget) {
    Collider *collider = malloc(sizeof(Collider));
    collider->widget.rect = rect;
    if (target_widget->type == WIDGET_TEXT)
    {
        collider->widget.rect.x = collider->widget.rect.x-20;
        collider->widget.rect.y = collider->widget.rect.y-20;
    }
    collider->widget.type = WIDGET_COLLIDER;
    collider->widget.personal_procedure = NULL;
    collider->widget.clicked = NULL;
    collider->target_widget = target_widget;
    collider->interacted_with = false;
    collider->hover = false;
    return collider;
}

Collider* create_collider_for(Widget *target_widget) {
    return make_collider_widget(target_widget->rect, target_widget);
}

// === Mainloop for testing ===

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);

    SDL_Window *window = SDL_CreateWindow("SDL2 GUI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font *font = TTF_OpenFont("assets/font.TTF", 24);
    if (!font) {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        return 1;
    }

    // Example GUI
    Gui gui = {.widget_count = 0};

    // Create a Box widget
    /*Box *background = malloc(sizeof(Box));
    background->widget.rect = (SDL_Rect){90, 40, 200, 50};
    background->widget.personal_procedure = NULL;
    background->widget.clicked = NULL;
    background->widget.type = WIDGET_BOX;
    background->widget.personal_procedure = change_color_on_hover;
    background->widget.color = (SDL_Color){0, 0, 255, 255};
    background->is_visible = true;
    background->bounds = background->widget.rect;*/
    Box * background = make_box_widget((SDL_Rect){100,10,40,30},(SDL_Color){100,29,179,255},true,change_color_on_hover);
    gui.widgets[gui.widget_count++] = (Widget *)background;

    // Create a Text widget using the new function
    Text *title = make_text_widget((SDL_Rect){100, 50, 280, 100}, "Hello, SDL2 GUI!", (SDL_Color){255, 255, 255, 255}, font, change_color_on_hover);
    gui.widgets[gui.widget_count++] = (Widget *)title;

    // Create Collider for the Text widget using the new function
    Collider *text_collider = create_collider_for((Widget *)title);
    gui.widgets[gui.widget_count++] = (Widget *)text_collider;

    Collider *button = create_collider_for((Widget *)background);
    gui.widgets[gui.widget_count++] = (Widget *)button;

    // Main loop
    bool running = true;
    SDL_Event e;
    gui_init(&gui);

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        interact_gui(&gui);
        update_gui(&gui,NULL);

        draw_gui_visible_components(&gui, renderer);

        SDL_RenderPresent(renderer);
    }

    // Cleanup
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}

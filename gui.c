//gui.c
#include "gui.h"
#include <math.h>
// === Specific procedures to free objects ===

void free_box(void * object) {
    free((Box*)object);
}

void free_collider(void * object) {
    free((Collider*)object);
}

void free_text(void * object) {
    Text* text = (Text*)object;
    if (text) {
        free(text->content);
        free(text);
    }
}

void free_image(void * object) {
    free((Image*)object);
}

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
}

void draw_image(SDL_Renderer *renderer, Image *image) {
    if (!image || !image->texture) return;
    const SDL_Rect dst = image->widget.rect;
    SDL_RenderCopy(renderer, image->texture, NULL, &dst);
}

void draw_gui_visible_components(const Gui *gui, SDL_Renderer *renderer){
    for (int i = 0; i < gui->widget_count; i++) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

        SDL_RenderFillRect(renderer, &(SDL_Rect){gui->w-1,gui->h-1,1,1});
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

bool if_text_bind(void * object) {
    Text* text = (Text*)object;
    if (!text) return false;
    text -> widget.upper_component = object;
    text -> widget.deletion_procedure = free_text;
    return true;
}

bool if_box_bind(void * object) {
    Box* box = (Box*)object;
    if (!box) return false;
    box->widget.upper_component = object;
    box->widget.deletion_procedure = free_box;
    return true;
}

bool if_collider_bind(void * object) {
    Collider* collider = (Collider*)object;
    if (!collider) return false;
    collider->widget.upper_component = object;
    collider->widget.deletion_procedure = free_collider;
    return true;
}

void bind_gui(const Gui *gui) {
    for (int i = 0; i < gui->widget_count; i++) {
        if (!if_box_bind(gui->widgets[i])){
            if (!if_text_bind(gui->widgets[i])) {
                if (!if_collider_bind(gui->widgets[i])) {
                    printf("wtf is that");
                }
            }
        }
    }
}

void gui_init(const Gui *gui)
{
    for (int i = 0; i < gui->widget_count; i++)
    {
        Widget *w = gui->widgets[i];
        if (!w) continue;
        w->origin = w->rect;
        w->default_color = w->color;
    }
    bind_gui(gui);
}

void free_gui(const Gui *gui) {
    for (int i = 0; i < gui->widget_count; i++) {
        Widget *w = gui->widgets[i];
        if (!w) continue;
        if (w->upper_component!=NULL&&w->deletion_procedure!=NULL) {
            w->deletion_procedure(w->upper_component);
        }else if (w->upper_component){
            free(w->upper_component);
        }else {
            free(w);
        }
    }
    printf("everything should be free\n");
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
        collider->hover = in_rect;
        collider->target_widget->selected = &collider->hover;
        collider->interacted_with = (in_rect && mouse_is_down);
        collider->target_widget->clicked = &collider->interacted_with;
    }
}

void update_gui(const Gui *gui, Context * context)
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

Text* make_text_widget(const SDL_Rect rect, const char *content, const SDL_Color color, TTF_Font *font, void (*personal_procedure)(Widget *self,Context * context)) {
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

Box * make_box_widget(const SDL_Rect rect, const SDL_Color color,const bool visible, void (*personal_procedure)(Widget *self, Context * context)) {
    Box *box = malloc(sizeof(Box));
    box->widget.rect = rect;
    box->widget.personal_procedure = personal_procedure;
    box->widget.clicked = NULL;
    box->is_visible = visible;
    box->widget.type = WIDGET_BOX;
    box->widget.color = color;
    return box;
}

Box * debug_box(Text* text){
	Box * box = malloc(sizeof(Box));
	box -> widget.rect = text -> widget.rect;
	box -> widget.rect.x = text -> widget.rect.x-20;
	box -> widget.rect.y  = text -> widget.rect.y-20;
	box	-> widget.personal_procedure=change_color_on_hover;
	box -> widget.clicked = NULL;
	box -> is_visible = true;
	box -> widget.type = WIDGET_BOX;
	box -> widget.color = (SDL_Color){100,100,100,100};
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

// === Root widget procedures ===

void change_color_on_hover(Widget *self, Context * context)
{
    if (*self->selected)
    {
        if ( *self->clicked)
        {
            //printf("tex_clicked");
            self->color.r = 100;
            self->color.g = 100;
            self->color.b = 50;
            self->color.a = 255;
        }
        else
        {
            //printf("tex_hover");
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

void change_size_on_click(Widget *self, Context * context)
{
    if (*self->clicked)
    {
        //printf("Self got clicked yayy\n");
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

typedef struct Context{
	bool running;
	bool playing;
	bool playingis_down;
	int time;
	int inertia;
}Context;

void animate(Widget *self,Context*context){
	context ->time = context->time%40;
	if (context->time!=0){return;}
	if (self->rect.y>self->origin.y+20){
		context -> inertia-=1;
	}else{
		self->rect.x=self->origin.x+context->inertia*context->inertia/20;		
		context -> inertia+=1;
	}
	printf("trying :(\n");
	//self->rect.x =self->rect.x + context->inertia;
	self->rect.y =self->rect.y + context->inertia;
}


void exit_on_click(Widget *self, Context * context) {
	//animate(self,context);
	if (*self->selected){
		self->color.r =0;
	}else{
		self->color.r =self->default_color.r;
	}
    if (*self->clicked) {
        context->running=false;
        printf("exiting...\n");
    }
}

void pause_unpause(Widget * self, Context*context){
	change_color_on_hover(self,context);
	if (*self ->clicked){
		if (!context -> playingis_down){
			context -> playing = !context -> playing;
		}
		context -> playingis_down = true;
	}else {
		context -> playingis_down =false;
	}
}

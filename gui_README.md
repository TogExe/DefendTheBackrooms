# Simple explanation of my gui system :
__
first of this framework is meant to help you create **interactive widgets** on the screen such as **textboxes** and simple **colored boxes**.
I will soon add image widgets.

## How to add a widget ? 

i'll guide you trough the steps of making a user interface


### Prepare sdl.

To add a widget you first off need your sdl window and your `renderer` will be necessary to use this system.
For text widgets you **might** also **need a font** wich can be made **like this** :
```c
TTF_Font *font = TTF_OpenFont("assets/font.TTF", 24);
    if (!font) {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        return 1;
    }
```


### Step 1 : set up a Gui.

a `Gui` structure looks overall like this :
```c
// GUI structure
typedef struct Gui {
    Widget *widgets[MAX_WIDGETS];  // Array of widgets
    int widget_count;              // The number of widgets
} Gui;
```
A `Gui` is set up like this :
```c
// Example GUI
Gui gui = {.widget_count = 0};
```
Now you have a gui



### Step 2 : adding your widgets.

The step one was easy right? now we will set up your widgets.

all you need is to do is :

```c
//exemple rectangle
Box * rectangle = make_box_widget((SDL_Rect){100,10,40,30},(SDL_Color){100,29,179,255},true,change_color_on_hove);
gui.widgets[gui.widget_count++] = (Widget *)rectangle;
```
`(SDL_Rect){100,10,40,30}` is the shape `(SDL_Rect){x,y,w,h}`

















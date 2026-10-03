#ifndef UI_H
#define UI_H

typedef struct UIElement UIElement;

typedef void (*UIActivateFunc)(UIElement *element);

struct UIElement
{
    /* geometry */
    int x;
    int y;
    int width;
    int height;

    /* identity */
    int id;

    /* state */
    int enabled;
    int focused;

    /* presentation */
    unsigned int normal_color;
    unsigned int focused_color;
    unsigned int activated_color;

    /* behavior */
    UIActivateFunc on_activate;
    void *user_data;
};

void ui_element_draw(const UIElement *element);
void ui_element_activate(UIElement *element);

#endif
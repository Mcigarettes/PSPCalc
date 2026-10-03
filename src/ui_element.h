#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

/*
 * v0.3.0: UIElement moved out of ui.h into its own module.
 *
 * A UIElement is a rectangular, focusable, activatable region.
 * It does NOT know:
 *   - where input comes from
 *   - how focus navigation works
 *   - how many siblings exist
 */

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
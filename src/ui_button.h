#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "ui_element.h"

/*
 * v0.3.1: UIButton — a UIElement with visual button states.
 * v0.3.2: buttons carry an optional text label, rendered
 *         centered via the built-in bitmap font.
 *
 * UIButton specializes UIElement. Its 'base' field MUST remain
 * the first member so that a UIButton* can be safely cast to
 * UIElement*.
 */

#define UI_BUTTON_PRESS_FRAMES 15

typedef struct UIButton UIButton;

typedef void (*UIButtonOnActivate)(UIButton *button);

struct UIButton
{
    /* must be first: allows (UIElement *)button */
    UIElement base;

    /* button-specific visual state */
    int pressed;
    int pressed_frames;

    /* colors */
    unsigned int pressed_color;

    /* v0.3.2: optional text */
    const char *text;
    unsigned int text_color;

    /* button-level callback */
    UIButtonOnActivate on_activate;
};

void ui_button_init(
    UIButton *button,
    int x,
    int y,
    int width,
    int height,
    int id
);

void ui_button_set_text(UIButton *button, const char *text);

void ui_button_update(UIButton *button);

void ui_button_draw(const UIButton *button);

void ui_button_activate(UIButton *button);

#endif
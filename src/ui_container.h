#ifndef UI_CONTAINER_H
#define UI_CONTAINER_H

#include "ui_element.h"

/*
 * v0.2.7: Focus / Container layer.
 * v0.3.0: include path changed from ui.h to ui_element.h.
 * v0.3.1: elements is now UIElement ** (array of pointers),
 *         because UIButton is larger than UIElement and
 *         cannot be stored in a contiguous UIElement array.
 */

typedef enum
{
    UI_DIR_UP = 0,
    UI_DIR_DOWN,
    UI_DIR_LEFT,
    UI_DIR_RIGHT
} UIDirection;

typedef struct
{
    UIElement **elements;
    int count;
    int cols;
    int focus_index;
} UIContainer;

void ui_container_init(
    UIContainer *container,
    UIElement **elements,
    int count,
    int cols
);

int ui_container_get_focus_index(const UIContainer *container);

UIElement *ui_container_get_focused(UIContainer *container);

void ui_container_move_focus(
    UIContainer *container,
    UIDirection direction
);

#endif
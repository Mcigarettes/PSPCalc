#ifndef UI_CONTAINER_H
#define UI_CONTAINER_H

#include "ui_element.h"

/*
 * v0.2.7: Focus / Container layer.
 * v0.3.0: include path changed from ui.h to ui_element.h
 *         to avoid a circular aggregation dependency.
 *
 * UIContainer owns:
 *   - a pointer to an array of UIElement
 *   - the current focus index
 *   - the grid shape used for navigation
 *
 * It does NOT own:
 *   - the elements themselves
 *   - input handling
 *   - rendering
 *   - cursor / mouse-style interaction
 *
 * Direction is a UI-layer concept, deliberately NOT UIEventType,
 * so the UI layer never depends on the input layer.
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
    UIElement *elements;
    int count;
    int cols;
    int focus_index;
} UIContainer;

void ui_container_init(
    UIContainer *container,
    UIElement *elements,
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
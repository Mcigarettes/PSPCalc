#include <stddef.h>

#include "ui_container.h"

void ui_container_init(
    UIContainer *container,
    UIElement **elements,
    int count,
    int cols
)
{
    if (!container)
    {
        return;
    }

    container->elements = elements;
    container->count = count;
    container->cols = cols;
    container->focus_index = 0;
}

int ui_container_get_focus_index(const UIContainer *container)
{
    if (!container)
    {
        return -1;
    }

    return container->focus_index;
}

UIElement *ui_container_get_focused(UIContainer *container)
{
    if (!container)
    {
        return NULL;
    }

    if (container->focus_index < 0 ||
        container->focus_index >= container->count)
    {
        return NULL;
    }

    return container->elements[container->focus_index];
}

void ui_container_move_focus(
    UIContainer *container,
    UIDirection direction
)
{
    int rows;
    int row;
    int col;

    if (!container || container->cols <= 0 || container->count <= 0)
    {
        return;
    }

    rows = container->count / container->cols;
    row = container->focus_index / container->cols;
    col = container->focus_index % container->cols;

    switch (direction)
    {
    case UI_DIR_UP:
        if (row > 0)
        {
            container->focus_index -= container->cols;
        }
        break;

    case UI_DIR_DOWN:
        if (row < rows - 1)
        {
            container->focus_index += container->cols;
        }
        break;

    case UI_DIR_LEFT:
        if (col > 0)
        {
            container->focus_index--;
        }
        break;

    case UI_DIR_RIGHT:
        if (col < container->cols - 1)
        {
            container->focus_index++;
        }
        break;

    default:
        break;
    }
}
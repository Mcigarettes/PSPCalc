#include <stddef.h>

#include "ui_container.h"

void ui_container_init(
    UIContainer *container,
    UIElement **elements,
    int count
)
{
    if (!container)
    {
        return;
    }

    container->elements = elements;
    container->count = count;
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

/*
 * v0.3.4: geometric focus navigation.
 */

static int ui_abs(int v)
{
    return v < 0 ? -v : v;
}

static int ui_center_x(const UIElement *e)
{
    return e->x + e->width / 2;
}

static int ui_center_y(const UIElement *e)
{
    return e->y + e->height / 2;
}

void ui_container_move_focus(
    UIContainer *container,
    UIDirection direction
)
{
    UIElement *current;
    int cur_cx;
    int cur_cy;
    int best_index;
    int best_score;

    if (!container ||
        container->count <= 0 ||
        container->focus_index < 0 ||
        container->focus_index >= container->count)
    {
        return;
    }

    current = container->elements[container->focus_index];

    if (!current)
    {
        return;
    }

    cur_cx = ui_center_x(current);
    cur_cy = ui_center_y(current);

    best_index = -1;
    best_score = 0;

    for (int i = 0; i < container->count; i++)
    {
        UIElement *e;
        int dx;
        int dy;
        int in_direction;
        int primary;
        int perp;
        int score;

        if (i == container->focus_index)
        {
            continue;
        }

        e = container->elements[i];

        if (!e || !e->enabled)
        {
            continue;
        }

        dx = ui_center_x(e) - cur_cx;
        dy = ui_center_y(e) - cur_cy;

        switch (direction)
        {
        case UI_DIR_UP:    in_direction = (dy < 0); break;
        case UI_DIR_DOWN:  in_direction = (dy > 0); break;
        case UI_DIR_LEFT:  in_direction = (dx < 0); break;
        case UI_DIR_RIGHT: in_direction = (dx > 0); break;
        default:           in_direction = 0;        break;
        }

        if (!in_direction)
        {
            continue;
        }

        switch (direction)
        {
        case UI_DIR_UP:
        case UI_DIR_DOWN:
            primary = ui_abs(dy);
            perp = ui_abs(dx);
            break;
        default:
            primary = ui_abs(dx);
            perp = ui_abs(dy);
            break;
        }

        /*
         * Primary axis distance dominates; perpendicular
         * misalignment is weighted heavier, so a well-aligned
         * element further away beats a misaligned closer one.
         */
        score = primary + 2 * perp;

        if (best_index < 0 || score < best_score)
        {
            best_score = score;
            best_index = i;
        }
    }

    if (best_index >= 0)
    {
        container->focus_index = best_index;
    }
}

/*
 * v0.3.5: cursor hit test against container elements.
 */
int ui_container_hit_test(
    const UIContainer *container,
    int x,
    int y
)
{
    if (!container || container->count <= 0)
    {
        return -1;
    }

    for (int i = 0; i < container->count; i++)
    {
        const UIElement *e = container->elements[i];

        if (!e || !e->enabled)
        {
            continue;
        }

        if (x >= e->x &&
            x <  e->x + e->width &&
            y >= e->y &&
            y <  e->y + e->height)
        {
            return i;
        }
    }

    return -1;
}
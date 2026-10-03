#include <stddef.h>

#include "ui_system.h"

void ui_system_init(
    UISystem *sys,
    UIElement **elements,
    int count
)
{
    if (!sys)
    {
        return;
    }

    ui_container_init(&sys->container, elements, count);

    sys->cursor_mode = 0;
    sys->hovered_index = -1;
    sys->cursor_x = 0;
    sys->cursor_y = 0;
}

void ui_system_handle_event(UISystem *sys, const UIEvent *event)
{
    if (!sys || !event)
    {
        return;
    }

    /* Always remember the latest cursor position */
    sys->cursor_x = (int)event->cursor_x;
    sys->cursor_y = (int)event->cursor_y;

    /* Analog movement switches selection to cursor mode */
    if (event->cursor_moved)
    {
        sys->cursor_mode = 1;
    }

    /* D-pad switches back to focus mode and moves focus */
    switch (event->type)
    {
    case UI_EVENT_UP:
        ui_container_move_focus(&sys->container, UI_DIR_UP);
        sys->cursor_mode = 0;
        break;

    case UI_EVENT_DOWN:
        ui_container_move_focus(&sys->container, UI_DIR_DOWN);
        sys->cursor_mode = 0;
        break;

    case UI_EVENT_LEFT:
        ui_container_move_focus(&sys->container, UI_DIR_LEFT);
        sys->cursor_mode = 0;
        break;

    case UI_EVENT_RIGHT:
        ui_container_move_focus(&sys->container, UI_DIR_RIGHT);
        sys->cursor_mode = 0;
        break;

    default:
        break;
    }

    /* Recompute hover from current cursor position */
    sys->hovered_index = ui_container_hit_test(
        &sys->container,
        sys->cursor_x,
        sys->cursor_y
    );
}

void ui_system_update(UISystem *sys)
{
    (void)sys;
    /* Reserved for future UI-wide per-frame updates. */
}

int ui_system_get_active_index(const UISystem *sys)
{
    if (!sys)
    {
        return -1;
    }

    if (sys->cursor_mode && sys->hovered_index >= 0)
    {
        return sys->hovered_index;
    }

    return ui_container_get_focus_index(&sys->container);
}

int ui_system_get_focus_index(const UISystem *sys)
{
    if (!sys)
    {
        return -1;
    }

    return ui_container_get_focus_index(&sys->container);
}

UIElement *ui_system_get_active_element(const UISystem *sys)
{
    int idx;
    UIElement *element;

    if (!sys)
    {
        return NULL;
    }

    idx = ui_system_get_active_index(sys);

    if (idx < 0 || idx >= sys->container.count)
    {
        return NULL;
    }

    element = sys->container.elements[idx];

    return element;
}

int ui_system_get_cursor_x(const UISystem *sys)
{
    return sys ? sys->cursor_x : 0;
}

int ui_system_get_cursor_y(const UISystem *sys)
{
    return sys ? sys->cursor_y : 0;
}
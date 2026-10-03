#ifndef UI_SYSTEM_H
#define UI_SYSTEM_H

#include "event.h"
#include "ui_element.h"
#include "ui_container.h"

/*
 * v0.3.6: UISystem — top-level UI state coordinator.
 *
 * Packages the UI state that used to live in main.c:
 *   - the focus container
 *   - cursor_mode  (1 = analog cursor is driving selection)
 *   - hovered_index (element under the cursor, or -1)
 *   - last seen cursor position (x, y)
 *
 * UISystem does NOT own the elements themselves; they are
 * provided by the app at init time. It also does NOT draw
 * anything; drawing stays with each UI element's own draw
 * function.
 *
 * The app queries ui_system_get_active_index() to know which
 * element should visually appear focused, and activates that
 * element when it receives UI_EVENT_ACTIVATE.
 */

typedef struct
{
    UIContainer container;

    int cursor_mode;
    int hovered_index;
    int cursor_x;
    int cursor_y;
} UISystem;

void ui_system_init(
    UISystem *sys,
    UIElement **elements,
    int count
);

/*
 * Update internal state from an input event.
 * Does NOT dispatch activation; the app reads
 * ui_system_get_active_index() and activates accordingly.
 */
void ui_system_handle_event(UISystem *sys, const UIEvent *event);

/*
 * Per-frame update. Reserved for future UI-wide timers;
 * currently a no-op.
 */
void ui_system_update(UISystem *sys);

/*
 * Active index:
 *   - if cursor_mode and cursor is over an element: that element
 *   - otherwise: current focus index
 * Returns -1 if no valid element.
 */
int ui_system_get_active_index(const UISystem *sys);

int ui_system_get_focus_index(const UISystem *sys);

UIElement *ui_system_get_active_element(const UISystem *sys);

int ui_system_get_cursor_x(const UISystem *sys);
int ui_system_get_cursor_y(const UISystem *sys);

#endif
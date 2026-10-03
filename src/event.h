#ifndef EVENT_H
#define EVENT_H

/*
 * v0.3.6: UIEvent is the shared protocol between the input layer
 * and the UI layer.
 *
 * It lives in its own header so that:
 *   - input.h can be included by the input layer without pulling
 *     in any UI type
 *   - ui_system.h can consume UIEvent without including input.h
 *
 * Neither input.h nor any ui_*.h should ever include each other.
 */

typedef enum
{
    UI_EVENT_NONE = 0,
    UI_EVENT_MOVE,
    UI_EVENT_ACTIVATE,
    UI_EVENT_BACK,
    UI_EVENT_EXIT,
    UI_EVENT_UP,
    UI_EVENT_DOWN,
    UI_EVENT_LEFT,
    UI_EVENT_RIGHT
} UIEventType;

typedef struct
{
    UIEventType type;
    float cursor_x;
    float cursor_y;
    int cursor_moved;   /* v0.2.5: analog stick moved the cursor this frame */
} UIEvent;

#endif
#ifndef INPUT_H
#define INPUT_H

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
} UIEvent;

void input_init(void);
UIEvent input_update(void);

#endif
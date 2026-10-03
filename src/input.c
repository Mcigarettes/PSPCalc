#include <pspctrl.h>

#include "input.h"

#define SCREEN_WIDTH  480
#define SCREEN_HEIGHT 272

static float cursor_x = 240.0f;
static float cursor_y = 136.0f;
static unsigned int previous_buttons = 0;

void input_init(void)
{
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
}

UIEvent input_update(void)
{
    SceCtrlData pad;
    UIEvent event;

    event.type = UI_EVENT_NONE;
    event.cursor_x = cursor_x;
    event.cursor_y = cursor_y;
    event.cursor_moved = 0;   /* v0.2.5 */

    sceCtrlPeekBufferPositive(
        &pad,
        1
    );

    /*
     * Analog stick
     */
    float dx = (float)pad.Lx - 128.0f;
    float dy = (float)pad.Ly - 128.0f;

    /*
     * Dead zone
     */
    if (dx > -15.0f && dx < 15.0f)
    {
        dx = 0.0f;
    }

    if (dy > -15.0f && dy < 15.0f)
    {
        dy = 0.0f;
    }

    /* v0.2.5: report analog movement to UI layer */
    if (dx != 0.0f || dy != 0.0f)
    {
        event.cursor_moved = 1;
    }

    /*
     * Movement speed
     */
    cursor_x += dx * 0.08f;
    cursor_y += dy * 0.08f;

    /*
     * Keep cursor on screen
     */
    if (cursor_x < 10.0f)
        cursor_x = 10.0f;

    if (cursor_x > SCREEN_WIDTH - 10.0f)
        cursor_x = SCREEN_WIDTH - 10.0f;

    if (cursor_y < 10.0f)
        cursor_y = 10.0f;

    if (cursor_y > SCREEN_HEIGHT - 10.0f)
        cursor_y = SCREEN_HEIGHT - 10.0f;

    event.cursor_x = cursor_x;
    event.cursor_y = cursor_y;

    if ((pad.Buttons & PSP_CTRL_START) &&
        !(previous_buttons & PSP_CTRL_START))
    {
        event.type = UI_EVENT_EXIT;
    }
    else if ((pad.Buttons & PSP_CTRL_CROSS) &&
             !(previous_buttons & PSP_CTRL_CROSS))
    {
        event.type = UI_EVENT_ACTIVATE;
    }
    else if ((pad.Buttons & PSP_CTRL_CIRCLE) &&
             !(previous_buttons & PSP_CTRL_CIRCLE))
    {
        event.type = UI_EVENT_BACK;
    }
    else if ((pad.Buttons & PSP_CTRL_UP) &&
             !(previous_buttons & PSP_CTRL_UP))
    {
        event.type = UI_EVENT_UP;
    }
    else if ((pad.Buttons & PSP_CTRL_DOWN) &&
             !(previous_buttons & PSP_CTRL_DOWN))
    {
        event.type = UI_EVENT_DOWN;
    }
    else if ((pad.Buttons & PSP_CTRL_LEFT) &&
             !(previous_buttons & PSP_CTRL_LEFT))
    {
        event.type = UI_EVENT_LEFT;
    }
    else if ((pad.Buttons & PSP_CTRL_RIGHT) &&
             !(previous_buttons & PSP_CTRL_RIGHT))
    {
        event.type = UI_EVENT_RIGHT;
    }

    previous_buttons = pad.Buttons;

    return event;
}
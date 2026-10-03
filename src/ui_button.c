#include <pspgu.h>

#include "ui_button.h"
#include "ui_text.h"

typedef struct
{
    unsigned int color;
    short x;
    short y;
    short z;
} ButtonVertex;

static void button_draw_rect(
    int x,
    int y,
    int width,
    int height,
    unsigned int color
)
{
    ButtonVertex *v;

    v = (ButtonVertex *)sceGuGetMemory(2 * sizeof(ButtonVertex));

    v[0].color = color;
    v[0].x = x;
    v[0].y = y;
    v[0].z = 0;

    v[1].color = color;
    v[1].x = x + width;
    v[1].y = y + height;
    v[1].z = 0;

    sceGuDrawArray(
        GU_SPRITES,
        GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D,
        2,
        NULL,
        v
    );
}

void ui_button_init(
    UIButton *button,
    int x,
    int y,
    int width,
    int height,
    int id
)
{
    if (!button)
    {
        return;
    }

    button->base.x = x;
    button->base.y = y;
    button->base.width = width;
    button->base.height = height;
    button->base.id = id;
    button->base.enabled = 1;
    button->base.focused = 0;

    button->base.normal_color = 0xFF30405A;
    button->base.focused_color = 0xFF8060FF;
    button->base.activated_color = 0xFF00FF00;

    button->base.on_activate = NULL;
    button->base.user_data = NULL;

    button->pressed = 0;
    button->pressed_frames = 0;
    button->pressed_color = 0xFF00FF00;

    button->text = NULL;
    button->text_color = 0xFFFFFFFF;

    button->on_activate = NULL;
}

void ui_button_set_text(UIButton *button, const char *text)
{
    if (!button)
    {
        return;
    }

    button->text = text;
}

void ui_button_update(UIButton *button)
{
    if (!button)
    {
        return;
    }

    if (button->pressed_frames > 0)
    {
        button->pressed_frames--;

        if (button->pressed_frames == 0)
        {
            button->pressed = 0;
        }
    }
}

void ui_button_draw(const UIButton *button)
{
    unsigned int color;

    if (!button || !button->base.enabled)
    {
        return;
    }

    if (button->pressed)
    {
        color = button->pressed_color;
    }
    else if (button->base.focused)
    {
        color = button->base.focused_color;
    }
    else
    {
        color = button->base.normal_color;
    }

    button_draw_rect(
        button->base.x,
        button->base.y,
        button->base.width,
        button->base.height,
        color
    );

    /* v0.3.2: centered text */
    if (button->text)
    {
        int scale = 1;
        int tw = ui_text_width(button->text, scale);
        int th = ui_text_height(scale);

        int tx = button->base.x + (button->base.width - tw) / 2;
        int ty = button->base.y + (button->base.height - th) / 2;

        ui_text_draw(
            tx,
            ty,
            button->text,
            button->text_color,
            scale
        );
    }
}

void ui_button_activate(UIButton *button)
{
    if (!button || !button->base.enabled)
    {
        return;
    }

    button->pressed = 1;
    button->pressed_frames = UI_BUTTON_PRESS_FRAMES;

    if (button->on_activate)
    {
        button->on_activate(button);
    }
}
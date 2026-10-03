#include <pspgu.h>

#include "ui.h"

typedef struct
{
    unsigned int color;
    short x;
    short y;
    short z;
} UIVertex;

static void ui_draw_rect(
    int x,
    int y,
    int width,
    int height,
    unsigned int color
)
{
    UIVertex *vertices;

    vertices = (UIVertex *)sceGuGetMemory(2 * sizeof(UIVertex));

    vertices[0].color = color;
    vertices[0].x = x;
    vertices[0].y = y;
    vertices[0].z = 0;

    vertices[1].color = color;
    vertices[1].x = x + width;
    vertices[1].y = y + height;
    vertices[1].z = 0;

    sceGuDrawArray(
        GU_SPRITES,
        GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D,
        2,
        NULL,
        vertices
    );
}

void ui_element_draw(const UIElement *element)
{
    unsigned int color;

    if (!element || !element->enabled)
    {
        return;
    }

    if (element->focused)
    {
        color = element->focused_color;
    }
    else
    {
        color = element->normal_color;
    }

    ui_draw_rect(
        element->x,
        element->y,
        element->width,
        element->height,
        color
    );
}

void ui_element_activate(UIElement *element)
{
    if (!element || !element->enabled)
    {
        return;
    }

    if (element->on_activate)
    {
        element->on_activate(element);
    }
}
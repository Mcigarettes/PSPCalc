#include <pspgu.h>

#include "ui_panel.h"
#include "ui_text.h"

typedef struct
{
    unsigned int color;
    short x;
    short y;
    short z;
} PanelVertex;

static void panel_draw_rect(
    int x,
    int y,
    int width,
    int height,
    unsigned int color
)
{
    PanelVertex *v;

    v = (PanelVertex *)sceGuGetMemory(2 * sizeof(PanelVertex));

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

void ui_panel_init(
    UIPanel *panel,
    int x,
    int y,
    int width,
    int height,
    unsigned int background_color
)
{
    if (!panel)
    {
        return;
    }

    panel->x = x;
    panel->y = y;
    panel->width = width;
    panel->height = height;

    panel->background_color = background_color;

    panel->draw_border = 0;
    panel->border_color = 0xFF445577;

    panel->title = NULL;
    panel->title_color = 0xFFFFFFFF;
    panel->title_scale = 1;
    panel->title_offset_x = 12;
    panel->title_offset_y = 8;
}

void ui_panel_set_title(UIPanel *panel, const char *title)
{
    if (!panel)
    {
        return;
    }

    panel->title = title;
}

void ui_panel_draw(const UIPanel *panel)
{
    if (!panel)
    {
        return;
    }

    /* Background */
    panel_draw_rect(
        panel->x,
        panel->y,
        panel->width,
        panel->height,
        panel->background_color
    );

    /* Optional 1px border */
    if (panel->draw_border)
    {
        unsigned int c = panel->border_color;

        panel_draw_rect(panel->x, panel->y,
                        panel->width, 1, c);
        panel_draw_rect(panel->x, panel->y + panel->height - 1,
                        panel->width, 1, c);
        panel_draw_rect(panel->x, panel->y,
                        1, panel->height, c);
        panel_draw_rect(panel->x + panel->width - 1, panel->y,
                        1, panel->height, c);
    }

    /* Optional title */
    if (panel->title)
    {
        ui_text_draw(
            panel->x + panel->title_offset_x,
            panel->y + panel->title_offset_y,
            panel->title,
            panel->title_color,
            panel->title_scale
        );
    }
}

int ui_panel_local_x(const UIPanel *panel, int local_x)
{
    if (!panel)
    {
        return local_x;
    }

    return panel->x + local_x;
}

int ui_panel_local_y(const UIPanel *panel, int local_y)
{
    if (!panel)
    {
        return local_y;
    }

    return panel->y + local_y;
}
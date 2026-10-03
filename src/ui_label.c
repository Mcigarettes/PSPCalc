#include "ui_label.h"
#include "ui_text.h"

void ui_label_init(
    UILabel *label,
    int x,
    int y,
    const char *text,
    unsigned int color,
    int scale
)
{
    if (!label)
    {
        return;
    }

    label->x = x;
    label->y = y;
    label->text = text;
    label->color = color;
    label->scale = scale < 1 ? 1 : scale;
}

void ui_label_set_text(UILabel *label, const char *text)
{
    if (!label)
    {
        return;
    }

    label->text = text;
}

void ui_label_draw(const UILabel *label)
{
    if (!label || !label->text)
    {
        return;
    }

    ui_text_draw(
        label->x,
        label->y,
        label->text,
        label->color,
        label->scale
    );
}
#ifndef UI_LABEL_H
#define UI_LABEL_H

/*
 * v0.3.2: UILabel — a static text region.
 *
 * UILabel is deliberately NOT a UIElement subclass:
 * it is not focusable, not activatable, and not part of
 * the UIContainer focus graph. It is a pure display object.
 *
 * If we later need focusable/selectable labels, we can
 * revisit that decision. For now, keeping it separate
 * keeps the focus system clean.
 */

typedef struct
{
    int x;
    int y;
    const char *text;
    unsigned int color;
    int scale;
} UILabel;

void ui_label_init(
    UILabel *label,
    int x,
    int y,
    const char *text,
    unsigned int color,
    int scale
);

void ui_label_set_text(UILabel *label, const char *text);

void ui_label_draw(const UILabel *label);

#endif
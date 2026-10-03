#ifndef UI_PANEL_H
#define UI_PANEL_H

/*
 * v0.3.3: UIPanel — visual grouping of UI.
 *
 * Naming:
 *   UIContainer (from v0.2.7) is the FOCUS container.
 *   UIPanel (this file) is a VISUAL container.
 *   They are deliberately separate concepts.
 *
 * A UIPanel:
 *   - is NOT a UIElement
 *   - is NOT focusable
 *   - is NOT activatable
 *   - is NOT part of the focus graph
 *   - just draws a background, optional border, optional title,
 *     and provides a coordinate frame for children
 *
 * Automatic layout of children comes in v0.3.4.
 * For now, children are still positioned by absolute screen coords.
 */

typedef struct
{
    int x;
    int y;
    int width;
    int height;

    unsigned int background_color;

    int draw_border;
    unsigned int border_color;

    const char *title;
    unsigned int title_color;
    int title_scale;
    int title_offset_x;
    int title_offset_y;
} UIPanel;

void ui_panel_init(
    UIPanel *panel,
    int x,
    int y,
    int width,
    int height,
    unsigned int background_color
);

void ui_panel_set_title(UIPanel *panel, const char *title);

void ui_panel_draw(const UIPanel *panel);

int ui_panel_local_x(const UIPanel *panel, int local_x);
int ui_panel_local_y(const UIPanel *panel, int local_y);

#endif
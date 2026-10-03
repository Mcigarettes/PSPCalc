#ifndef UI_TEXT_H
#define UI_TEXT_H

/*
 * v0.3.2: minimal built-in bitmap text renderer.
 *
 * Font: 5x7, one glyph per supported ASCII character.
 * No external font libraries.
 *
 * Coordinates: top-left origin, screen pixel coordinates.
 * scale: 1 = 5x7, 2 = 10x14, etc.
 */

void ui_text_draw(
    int x,
    int y,
    const char *text,
    unsigned int color,
    int scale
);

int ui_text_width(const char *text, int scale);

int ui_text_height(int scale);

#endif
# PSPCalc — Design

## Positioning

Two equal goals:
1. Working calculator for PSP.
2. Material Design 3-inspired visual system.

Not MD3-compliant. Adapted for 480x272, gamepad-only, no touch.

## Platform

| Item | Value |
|---|---|
| Resolution | 480 x 272 |
| Input | Analog stick, D-pad, X, O, Triangle, Square, L, R, START |
| Touch | none |
| Font | 5x7 bitmap (lowercase and punctuation to be added) |
| GPU | GU, 2D triangles |
| System theme | not readable |

## Visual Direction

- No top app bar (too tall).
- Pill-shaped buttons (radius = height / 2).
- Elevation simulated by color value differences.
- Reserved, minimal animation. Linear frame steps only.
- Decor elements (arc, wave) drawn as high-density small rectangles,
  not baked bitmaps.
- All art in code. No external asset files.

## Color

MD3 color roles:

    primary / on-primary
    primary-container / on-primary-container
    secondary / on-secondary
    secondary-container / on-secondary-container
    surface / on-surface
    surface-container / on-surface-container
    outline

Values come from a UITheme (introduced in v0.6.0). No hardcoding
of colors in component code after v0.6.0.

Preset themes:

    PURPLE (MD3 default)
    BLUE
    GREEN
    RED
    AMBER
    MONO (black and white)

## Font

- 5x7 bitmap, built in.
- Uppercase, lowercase, digits, common punctuation.
- Bold is a fake bold: render twice, second pass offset by (scale, 0).
- No vector fonts.

## Icons

- 12x12 bitmap, built in.
- 13 icons: D-pad, Analog, X, O, Triangle, Square, L, R, START,
  Appearance, Settings, Help, About.
- Used in help page and overlay menu.

## Input Model

    PSP Controller
         v
       input.c           only file that touches pspctrl.h
         v
       UIEvent           shared protocol (event.h)
         v
       UISystem          owns focus state
         v
       UIAction / activate

Rules:
- input.c never includes ui_*.h.
- ui_*.c never includes pspctrl.h.
- The only shared type is UIEvent.

Analog stick behaves like D-pad:
- Push once -> one directional event.
- Hold -> key repeat after delay.
- Threshold 60 for push, 30 for release (hysteresis).

## UI Topology

    UISystem
      +-- UITheme
      +-- AppState (TITLE / CALCULATOR / APPEARANCE / SETTINGS / HELP / ABOUT)
      +-- Calculator overlay menu
            +-- Appearance
            +-- Settings
            +-- Help
            +-- About

AppState is a flat enum. ScreenManager is deferred to v0.6.6
unless a nested navigation requirement appears.

## Calculator Screen

    +-----------------------------------+
    |  Display panel                    |  primary (large, right-aligned)
    |                                   |  expression (small, right-aligned)
    +-----------------------------------+
    |  7   8   9   +   -                |
    |  4   5   6   *   /                |
    |  1   2   3   0   =                |
    +-----------------------------------+

- 5 x 3 grid.
- Button roles: DIGIT, OPERATOR, FUNCTION, EQUALS.
- "=" uses primary color role.

## Physical Button Mapping

| Button | Function |
|---|---|
| D-pad / Analog | Move focus |
| X | Confirm |
| O | Back |
| Triangle | Open overlay menu |
| Square | Delete last digit |
| L | Clear all (AC) |
| R | Clear entry (C) |
| START | Title: enter calculator |

X / O swap is a Settings item (v0.4.6). The UI layer
always sees ACTIVATE / BACK; only input.c knows the
physical layout.

## Calculator Rules

- Input: integers only. Max 10 digits.
- Results: may show decimals. Not limited in value.
- DEL: input state -> remove last digit. Single digit -> empty.
  Result state -> no effect. Just after operator -> remove operator.
- Operators: pressing a second one replaces the first.
- "=": pressing again repeats the last operation.
- Divide by zero -> "ERROR".
- Long results: largest font scale that fits, then truncate.
- No precision handling beyond double.

## Return Paths

- Sub-page O/X -> Calculator
- Calculator O/X -> Title
- Title START -> Calculator (state preserved)
- Overlay O/X -> close overlay, stay in Calculator

## Known Technical Debt

- Colors hardcoded in main.c (until v0.6.0).
- Font has no lowercase or punctuation (until v0.4.1).
- No icon system (until v0.4.1).
- No rounded rectangle primitive (until v0.6.1).
- No state layer (until v0.6.2).
- No bold font (until v0.6.3).
- main.c holds all UI elements directly (until v0.6.6 or later).
- No settings persistence (until v0.6.5).

## Explicitly Out of Scope

- Vector fonts
- Real shadows or blur
- Per-role custom color editing
- Random color generation
- Reading PSP system theme
- Touch or gestures
- 3D acceleration
- Scientific functions, parentheses, percent, memory
- Input history
- Decimal input

---

Last updated: before v0.4.0.
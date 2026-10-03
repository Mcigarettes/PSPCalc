# PSPCalc — Roadmap

Material Design 3-inspired calculator for the Sony PSP.
Two goals are equal: working calculator, and MD3 visual system.

---

## Completed

- v0.1.0  GU baseline
- v0.2.0  input.c / input.h refactor
- v0.2.1  X / O event verification
- v0.2.2  button edge detection
- v0.2.3  D-pad focus navigation
- v0.2.4  Focus + X activate
- v0.2.5  Unified cursor / focus activate
- v0.2.6  UIElement abstraction
- v0.2.7  UIContainer (focus container)
- v0.3.0  UI modularization
- v0.3.1  UIButton
- v0.3.2  UILabel + bitmap font
- v0.3.3  UIPanel (visual container)
- v0.3.4  Geometric focus navigation
- v0.3.5  Cursor hit testing abstraction
- v0.3.6  Input / UI decoupled via UISystem

## Current

v0.4.0 not started.

---

## v0.4.x — Structure + Visual Assets + Pages + Calculator

| Ver | Goal | Release |
|---|---|---|
| 0.4.0 | Structure: 5x3 keypad, UIAction, UIButtonRole, dual-label display, analog key-repeat, cursor deactivated | yes |
| 0.4.1 | Assets: lowercase letters, punctuation, 13 icons (12x12) | no |
| 0.4.2 | AppState + title screen (Pixel-style, MD3 decor) | yes |
| 0.4.3 | Overlay menu + 4 stub pages | yes |
| 0.4.4 | Digit input state machine (DEL / AC / C) | yes |
| 0.4.5 | Four operators, equals, edge cases | yes |
| 0.4.6 | Fill 4 pages: Appearance / Settings / Help / About | yes |

## v0.5.x — Calculator Logic

| Ver | Goal | Release |
|---|---|---|
| 0.5.0 | Extract calculator module from main.c | no |
| 0.5.1 | Continuous calculation, state cleanup | yes |
| 0.5.2 | Number formatting, long results | yes |
| 0.5.3 | Error handling and edge cases | no |
| 0.5.4 | Technical debt cleanup | no |

## v0.6.x — Material Design 3

| Ver | Goal | Release |
|---|---|---|
| 0.6.0 | UITheme + 4-6 preset palettes | yes |
| 0.6.1 | Rounded rectangle primitive (pill buttons) | yes |
| 0.6.2 | State layer (focus / pressed / hover) | yes |
| 0.6.3 | Typography (scale + bold variants) | yes |
| 0.6.4 | Decor system (arc / wave parameterized) | no |
| 0.6.5 | Settings persistence (settings.cfg) | yes |
| 0.6.6 | ScreenManager upgrade (if needed) | no |

## v0.7.x — Full Testing

- 0.7.0  Input matrix
- 0.7.1  Calculator functions
- 0.7.2  UI stress
- 0.7.3  PSP hardware stability

## v0.8.x — Cleanup

- 0.8.0  Thin main.c
- 0.8.1  Module dependency audit
- 0.8.2  Asset organization

## v0.9.x — RC

- 0.9.0  Feature freeze
- 0.9.1  Full hardware test
- 0.9.2  Final UI adjustments
- 0.9.3  Release candidate

## v1.0.0 — Stable

---

## Release Policy

- Every version: git commit + tag + local archive under `releases/vX.Y.Z/`
- GitHub Release: only for user-visible UI or behavior changes
- README / CHANGELOG record every version, including non-released ones

## Explicitly Out of Scope

- Scientific functions, parentheses, percent, memory keys
- Input history
- Decimal input (results may show decimals)
- Reading PSP system theme
- Per-role custom color editing, random color generation
- Vector fonts, real shadows, blur
- START to quit (use PSP Home button)
- Cursor as a free-moving pointer (data structures retained, inactive)
- Cursor speed settings, long-press X actions
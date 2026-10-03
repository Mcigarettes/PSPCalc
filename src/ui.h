#ifndef UI_H
#define UI_H

/*
 * v0.3.0: ui.h is now the UI layer aggregation header.
 *
 * It does NOT define anything itself.
 * It just pulls in the UI submodules, so main.c only needs:
 *
 *     #include "ui.h"
 *
 * When new UI modules are added (button, label, panel, ...),
 * include them here.
 */

#include "ui_element.h"
#include "ui_container.h"

#endif
#ifndef INPUT_H
#define INPUT_H

#include "event.h"

/*
 * v0.3.6: UIEventType / UIEvent moved to event.h.
 * input.h now only declares the input layer's own API.
 */

void input_init(void);
UIEvent input_update(void);

#endif
#ifndef KEYBIND_LISTENER_H
#define KEYBIND_LISTENER_H

#include <Carbon/Carbon.h>
#include <ApplicationServices/ApplicationServices.h>
#include <stdbool.h>
#include "parser.h"

// Function prototypes
bool HandleKeybind(CGKeyCode keyCode, CGEventFlags flags);
CGEventRef EventHandler(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *userInfo);
void SetupKeybindListener(Keybinding *bindings, int count);

#endif // KEYBIND_LISTENER_H


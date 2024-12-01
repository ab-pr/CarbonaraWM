#ifndef KEY_LISTEN_H
#define KEY_LISTEN_H

#include "parse_keys.h"
#include <Carbon/Carbon.h>
#include <ApplicationServices/ApplicationServices.h>
#include <vector>

void listen_keydown(const std::vector<Keybind>& binds);
CGEventRef event_handler(CGEventTapProxy, CGEventType, CGEventRef, void*);
bool handle_key_event(CGKeyCode, CGEventFlags);

#endif // KEY_LISTEN_H
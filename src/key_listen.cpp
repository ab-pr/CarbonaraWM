#include "key_listen.h"
#include <iostream>
#include <vector>

static std::vector<Keybind> keybindings;

bool
handle_key_event(CGKeyCode keyCode, CGEventFlags flags)
{
    for (const auto& bind : keybindings) {
        if (bind.key == keyCode && (bind.masks & flags) == bind.masks) {
            std::cout << "Executing command: " << bind.cmd << std::endl;
            system(bind.cmd.c_str());
            return true; // Suppress the event 😉
        }
    }
    return false;
}

CGEventRef
event_handler(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* userInfo)
{
    if (type == kCGEventKeyDown) {
        CGKeyCode keyCode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
        CGEventFlags flags = CGEventGetFlags(event);

        if (handle_key_event(keyCode, flags)) {
            return NULL; // Suppress the event
        }
    }
    return event;
}

void
listen_keydown(const std::vector<Keybind>& binds)
{
    keybindings = binds;

    const void* keys[] = { kAXTrustedCheckOptionPrompt };
    const void* values[] = { kCFBooleanTrue };
    CFDictionaryRef options = CFDictionaryCreate(NULL, keys, values, 1,
                                                 &kCFCopyStringDictionaryKeyCallBacks,
                                                 &kCFTypeDictionaryValueCallBacks);

    if (!AXIsProcessTrustedWithOptions(options)) {
        std::cerr << "Accessibility permissions required." << std::endl;
        exit(1);
    }

    CFRelease(options);

    // Alledgedlyyyyyy, im the secret recipe - yatchy
    CGEventMask eventMask = CGEventMaskBit(kCGEventKeyDown);
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGSessionEventTap,
        kCGHeadInsertEventTap,
        kCGEventTapOptionDefault,
        eventMask,
        event_handler,
        NULL);

    if (!eventTap) {
        std::cerr << "Failed to create event tap" << std::endl;
        exit(1);
    }

    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(
        kCFAllocatorDefault, eventTap, 0);

    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);
    CFRunLoopRun();
}
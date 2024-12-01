#include "keybind_listener.h"
#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define a global variable for keybindings
static Keybinding *keybindings = NULL;
static int keybinding_count = 0;

// Handle a keybind and return whether the event should be suppressed
bool HandleKeybind(CGKeyCode keyCode, CGEventFlags flags) {
    char key_combination[256];
    snprintf(key_combination, sizeof(key_combination), "%s%s%s%c",
             (flags & kCGEventFlagMaskCommand) ? "CMD : " : "",
             (flags & kCGEventFlagMaskShift) ? "SHIFT : " : "",
             (flags & kCGEventFlagMaskControl) ? "CTRL : " : "",
             (char)(keyCode + 'A')); // Map keyCode to a character

    // Check if the key combination matches any keybinding
    for (int i = 0; i < keybinding_count; i++) {
        if (strcmp(key_combination, keybindings[i].key_combination) == 0) {
            printf("Executing command: %s\n", keybindings[i].command);
            system(keybindings[i].command); // Execute the command
            return true;                   // Suppress the event
        }
    }

    return false;
}

// Event handler for key events
CGEventRef EventHandler(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *userInfo) {
    if (type == kCGEventKeyDown) {
        CGKeyCode keyCode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
        CGEventFlags flags = CGEventGetFlags(event);

        if (HandleKeybind(keyCode, flags)) {
            return NULL; // Suppress the event
        }
    }

    return event;
}

// Set up the keybind listener
void SetupKeybindListener(Keybinding *bindings, int count) {
    keybindings = bindings;
    keybinding_count = count;

    const void *keys[] = { kAXTrustedCheckOptionPrompt };
    const void *values[] = { kCFBooleanTrue };
    CFDictionaryRef options = CFDictionaryCreate(NULL, keys, values, 1,
                                                 &kCFCopyStringDictionaryKeyCallBacks,
                                                 &kCFTypeDictionaryValueCallBacks);

    if (!AXIsProcessTrustedWithOptions(options)) {
        fprintf(stderr, "Accessibility permissions are required.\n");
        exit(1);
    }

    CFRelease(options);

    CGEventMask eventMask = CGEventMaskBit(kCGEventKeyDown);
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGSessionEventTap,
        kCGHeadInsertEventTap,
        0,
        eventMask,
        (CGEventTapCallBack)EventHandler,
        NULL);

    if (!eventTap) {
        fprintf(stderr, "Failed to create event tap\n");
        exit(1);
    }

    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(
        kCFAllocatorDefault, eventTap, 0);

    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);

    CGEventTapEnable(eventTap, true);

    CFRunLoopRun();

    CFRelease(runLoopSource);
    CFRelease(eventTap);
}

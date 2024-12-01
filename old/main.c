#include "keybind_listener.h"
#include "parser.h"
#include <stdio.h>

int main() {
    const char *config_file = "keybindings.conf";
    int binding_count = 0;

    // Parse the keybindings from the configuration file
    Keybinding *bindings = parse_keybindings(config_file, &binding_count);

    // Print the parsed keybindings
    printf("Loaded Keybindings:\n");
    for (int i = 0; i < binding_count; i++) {
        printf("Key Combination: %s -> Command: %s\n",
               bindings[i].key_combination, bindings[i].command);
    }

    // Set up the keybind listener
    printf("Starting keybind listener...\n");
    SetupKeybindListener(bindings, binding_count);

    // Free the keybindings (Cleanup will not occur here unless the listener exits)
    free_keybindings(bindings, binding_count);

    return 0;
}

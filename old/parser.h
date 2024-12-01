#ifndef PARSER_H
#define PARSER_H

// Structure to store a keybinding and its command
typedef struct {
	char *key;
	char *modifier;
    char *command;         // Associated command string, e.g., "open_app_window Terminal"
} Keybinding;

// Function prototypes
char *trim_whitespace(char *str);
Keybinding *parse_keybindings(const char *filename, int *binding_count);
void free_keybindings(Keybinding *bindings, int count);

#endif // PARSER_H

#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to trim leading and trailing whitespace
char
*trim_whitespace(char *str)
{
    char *end;

    // Trim leading whitespace
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0) return str; // All spaces

    // Trim trailing whitespace
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    // Null-terminate the string
    *(end + 1) = '\0';

    return str;
}

// Helper function to process commands
char
*process_command(const char *raw_command)
{
    if (strncmp(raw_command, "open_app_window", 15) == 0) {
        // Translate to "osascript -e '...' <application>"
        const char *app_name = strchr(raw_command, ' ');
        if (app_name) {
            // Skip leading space in app_name
            while (*app_name == ' ') {
                app_name++;
            }

            // Format the osascript command
            char *processed_command = malloc(
                strlen("osascript -e \"tell application \\\"") +
                strlen(app_name) +
                strlen("\\\" to activate\" -e 'tell application \"System Events\" to keystroke \"n\" using command down'") +
                1
            );

            sprintf(processed_command,
                    "osascript -e \"tell application \\\"%s\\\" to activate\" -e 'tell application \"System Events\" to keystroke \"n\" using command down'",
                    app_name);

            return processed_command;
        }
    } else if (strncmp(raw_command, "open_app", 8) == 0) {
        // Translate to "open -a <application>"
        const char *app_name = strchr(raw_command, ' ');
        if (app_name) {
            // Skip leading space in app_name
            while (*app_name == ' ') {
                app_name++;
            }

            char *processed_command = malloc(strlen("open -a ") + strlen(app_name) + 1);
            sprintf(processed_command, "open -a %s", app_name);
            return processed_command;
        }
    } else if (strncmp(raw_command, "custom_cmd", 10) == 0) {
        // Extract the content inside []
        const char *start = strchr(raw_command, '[');
        const char *end = strchr(raw_command, ']');

        if (start && end && start < end) {
            size_t length = end - start - 1;
            char *processed_command = malloc(length + 1);
            strncpy(processed_command, start + 1, length);
            processed_command[length] = '\0';
            return processed_command;
        }
    }

    // If no special processing is needed, return the raw command
    return strdup(raw_command);
}

// Function to parse the configuration file
Keybinding
*parse_keybindings(const char *filename, int *binding_count)
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    Keybinding *bindings = NULL;
    char line[256];
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        // Trim the line
        char *trimmed = trim_whitespace(line);

        // Skip empty lines and comments
        if (trimmed[0] == '#' || trimmed[0] == '\0') continue;

        // Split the line into key combination and command
        char *delimiter = strchr(trimmed, '=');
        if (!delimiter) {
            fprintf(stderr, "Invalid line: %s\n", trimmed);
            continue;
        }

        // Split the line into key combination and command
        *delimiter = '\0'; // Replace '=' with '\0' to split
        char *key_combination = trim_whitespace(trimmed);
        char *command = trim_whitespace(delimiter + 1);

        // Parse the key combination into key and modifier
        char *modifier = NULL;
        char *key = strtok(key_combination, "+");
        if (key) {
            char *next_token = strtok(NULL, "+");
            if (next_token) {
                modifier = strdup(key);  // Modifier is the first part
                key = strdup(next_token); // Key is the second part
            } else {
                modifier = strdup("");  // No modifier
                key = strdup(key);      // Key is the only part
            }
        }

        // Process the command
        char *processed_command = process_command(command);

        // Allocate memory for the new keybinding
        bindings = realloc(bindings, sizeof(Keybinding) * (count + 1));
        bindings[count].key = key;
        bindings[count].modifier = modifier;
        bindings[count].command = processed_command;

        count++;
    }

    fclose(file);

    *binding_count = count;
    return bindings;
}

// Function to free the keybindings
void
free_keybindings(Keybinding *bindings, int count)
{
    for (int i = 0; i < count; i++) {
        free(bindings[i].key);
        free(bindings[i].modifier);
        free(bindings[i].command);
    }
    free(bindings);
}

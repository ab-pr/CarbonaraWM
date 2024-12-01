#include <Carbon/Carbon.h>
#include <iostream>

#include "parse_keys.h"

bool
check_line(const std::string& line)
{

    // Check if the line is empty
    if (line.empty()) {
        std::cout << "Skipping empty line" << std::endl;
        return false;
    }

    // Check if the line is a comment
    if (line[0] == '#') {
        std::cout << "Skipping comment line: " << line << std::endl;
        return false;
    }

    return true;
}

bool
fill_keybinds(Keybind *bind, const std::string& str) {
    if (!check_line(str)) {
        return false;
    }

    bind->masks = extract_masks(str);
    bind->key = extract_key(str);
    bind->cmd = extract_cmd(str);

    std::cout << "Parsed Keybind - Masks: " << bind->masks << ", Key: " << bind->key << ", Command: " << bind->cmd << std::endl;

    // Check if the keybind is valid
    if (bind->masks == 0 || bind->key == 0 || bind->cmd.empty()) {
        return false;
    }

    return true;
}

int
extract_masks(const std::string& str)
{
    int mask = 0;

    std::size_t start = str.find("mask=(");
    if (start == std::string::npos) {
        return mask;
    }
    start += 6;

    // Find the closing )
    std::size_t end = str.find(')', start);
    if (end == std::string::npos) {
        return mask;
    }

    std::string masks_str = str.substr(start, end - start);
    masks_str.erase(std::remove_if(masks_str.begin(), masks_str.end(), ::isspace), masks_str.end());

    std::size_t pos = 0;
    while (pos < masks_str.length()) {

        // Find position of next &
        std::size_t amp_pos = masks_str.find('&', pos);
        std::string token;

        if (amp_pos != std::string::npos) {
            token = masks_str.substr(pos, amp_pos - pos);
            pos = amp_pos + 1;
        } else {
            token = masks_str.substr(pos);
            pos = masks_str.length();
        }

        // Match token to mask
        if (token == "SHIFT") 		mask |= kCGEventFlagMaskShift;
        else if (token == "CTRL") 	mask |= kCGEventFlagMaskControl;
        else if (token == "CMD") 	mask |= kCGEventFlagMaskCommand;
    }

    return mask;
}

int
extract_key(const std::string& str)
{
    // Find the position of "key=("
    std::size_t start = str.find("key=(");
    if (start == std::string::npos) {
        return 0; // "key=(" not found
    }
    start += 5; // Move past "key=("

    // Find the closing ')'
    std::size_t end = str.find(')', start);
    if (end == std::string::npos) {
        return 0; // Closing ')' not found
    }

    // Extract the key substring
    std::string key_str = str.substr(start, end - start);

    // Ensure the key string is a single character
    if (key_str.length() != 1) {
        return 0; // Invalid key string
    }

    // Convert key string to key code using a switch statement
    char key_char = key_str[0];
    switch (key_char) {
        case 'A': return kVK_ANSI_A;
        case 'B': return kVK_ANSI_B;
        case 'C': return kVK_ANSI_C;
        case 'D': return kVK_ANSI_D;
        case 'E': return kVK_ANSI_E;
        case 'F': return kVK_ANSI_F;
        case 'G': return kVK_ANSI_G;
        case 'H': return kVK_ANSI_H;
        case 'I': return kVK_ANSI_I;
        case 'J': return kVK_ANSI_J;
        case 'K': return kVK_ANSI_K;
        case 'L': return kVK_ANSI_L;
        case 'M': return kVK_ANSI_M;
        case 'N': return kVK_ANSI_N;
        case 'O': return kVK_ANSI_O;
        case 'P': return kVK_ANSI_P;
        case 'Q': return kVK_ANSI_Q;
        case 'R': return kVK_ANSI_R;
        case 'S': return kVK_ANSI_S;
        case 'T': return kVK_ANSI_T;
        case 'U': return kVK_ANSI_U;
        case 'V': return kVK_ANSI_V;
        case 'W': return kVK_ANSI_W;
        case 'X': return kVK_ANSI_X;
        case 'Y': return kVK_ANSI_Y;
        case 'Z': return kVK_ANSI_Z;
        default: return 0; // Default to 0 if key not found
    }
}

std::string
extract_cmd(const std::string& str)
{
    // Find the position of "task=("
    std::size_t start = str.find("task=(");
    if (start == std::string::npos) {
        return ""; // "task=(" not found
    }
    start += 6; // Move past "task=("

    // Find the closing ')'
    std::size_t end = str.find(')', start);
    if (end == std::string::npos) {
        return ""; // Closing ')' not found
    }

    // Extract the command substring
    return str.substr(start, end - start);
}


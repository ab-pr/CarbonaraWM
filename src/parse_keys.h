#ifndef PARSE_KEYS_H
#define PARSE_KEYS_H

#include <string>

struct Keybind {
    int masks;
    int key;
    std::string cmd;
};

bool check_line(const std::string&);
bool fill_keybinds(Keybind*, const std::string&);
int extract_masks(const std::string&);
int extract_key(const std::string&);
std::string extract_cmd(const std::string&);

#endif // PARSE_KEYS_H
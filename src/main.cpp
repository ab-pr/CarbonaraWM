#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include "parse_keys.h"
#include "key_listen.h"

void
init_keybinds(std::vector<Keybind>& binds)
{
    std::string line;
    const char* home = std::getenv("HOME");
    if (home == nullptr) {
        std::cerr << "HOME environment variable not set\n Exiting..." << std::endl;
        exit(1);
    }
    std::string config_path = std::string(home) + "/.config/carbrc";
    std::ifstream file(config_path);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << config_path << std::endl;
        exit(1);
    }

    while (std::getline(file, line)) {
        Keybind bind = {0, 0, ""}; // Initialize Keybind structure
        if (fill_keybinds(&bind, line)) {
            binds.push_back(bind);
        }
    }
}

int
main(int argc, char *argv[])
{
    std::vector<Keybind> binds;
    init_keybinds(binds);

    listen_keydown(binds); // Call listen_keydown directly

    return 0;
}
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "parse_keys.h"
//#include "key_listen.h"

void init_keybinds(std::vector<Keybind>& binds) {
    std::string line;
    std::ifstream file("carbrc");

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file" << std::endl;
        exit(1);
    }

    while (std::getline(file, line)) {
        Keybind bind = {0, 0, ""}; // Initialize Keybind structure
        if (fill_keybinds(&bind, line)) {
            binds.push_back(bind);
        }
    }
}

void
run(void)
{
	std::vector<Keybind> binds;
	init_keybinds(binds);

	for (int i = 0; i < binds.size(); i++) {
		std::cout << "\n\nMasks: " << binds[i].masks << std::endl;
		std::cout << "Key: " << binds[i].key << std::endl;
		std::cout << "Command: " << binds[i].cmd << std::endl;
	}
}

int
main(int argc, char *argv[])
{
	run();
	return 0;
}

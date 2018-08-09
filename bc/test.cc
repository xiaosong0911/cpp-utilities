#include "bc.h"

#include <iostream>
#include <string>

int main() {
    std::string line;
    while (std::getline(std::cin, line)) {
        double r;
        if (bc::eval(line.c_str(), r)) {
            std::cout << r << std::endl;
        } else {
            std::cout << "eval failed" << std::endl;
        }
    }
}

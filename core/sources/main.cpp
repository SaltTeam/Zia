
#include <iostream>

#include "Core.hpp"

int main() {
    std::cout << "Hello, World!" << std::endl;
    auto core = Core::Core();
    core.run();
    return 0;
}
#include <iostream>

#include "first_app.hpp"

int main() {
    try {
        TEn::FirstApp app{};
        app.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

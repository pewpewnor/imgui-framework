#include <iostream>

#include "application.h"

int runApplication() {
    Application app;
    app.run();
    return 0;
}

int main() {
    try {
        return runApplication();
    } catch (const std::exception& error) {
        std::cerr << "Fatal error: " << error.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred" << std::endl;
        return 1;
    }
}

#include <iostream>
#include "app.hpp"


int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cout << "Invalid amount of arguments provided, just provide the file to edit" << std::endl;
        return 1;
    }

    App *app = new App(640, 640);

    if (app->init()) {
        std::cout << "Application initialization failed " << std::endl;
        return 1;
    }

    app->load_file(argv[1]);
    // Blocking, will run app until quit requested by client.
    app->run();
    // Run cleanup steps
    app->deinit();

    return EXIT_SUCCESS;
}


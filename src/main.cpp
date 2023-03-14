#include "app.hpp"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(
            stderr,
            "Invalid amount of arguments provided, just provide the file to "
            "edit\n"
        );
        return 1;
    }

    App *app = new App(640, 640);

    if (app->init()) {
        fprintf(stderr, "Application initialization failed\n");
        return 1;
    }

    app->load_file(argv[1]);
    app->run();
    app->deinit();

    return EXIT_SUCCESS;
}

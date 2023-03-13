#ifndef GTXT_APP_HPP
#define GTXT_APP_HPP

#include <string>
#include <SDL.h>
#include <ft2build.h>
#include FT_FREETYPE_H

enum AppInitCode {
    Ok = 0,
    Error = 1,
};

class App {
    public:
        int width, height, cursor_position;
        std::string content, filepath;

        SDL_Renderer *renderer;
        SDL_Window *window;

        FT_Library library;
        FT_Face face;

        AppInitCode init();

        void deinit();

        void load_file(std::string fp);

        void save_file();

        void run();
};


#endif //GTXT_APP_HPP

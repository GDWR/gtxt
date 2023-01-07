#ifndef GRED_APP_HPP
#define GRED_APP_HPP


#include <string>
#include <SDL.h>
#include <freetype/freetype.h>

enum AppInitCode {
    Ok = 0,
    Error = 1,
};

class App {
public:
    uint width, height, cursor_position;
    std::string content, filepath;

    SDL_Renderer *renderer;
    SDL_Window *window;

    FT_Library library;
    FT_Face face;

    explicit App(uint width, uint height) : width(width), height(height) {}

    AppInitCode init();

    void deinit();

    void load_file(std::string fp);

    void save_file();

    void run();
};


#endif //GRED_APP_HPP

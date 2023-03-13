#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include "gui/sdl_exts.hpp"
#include "app.hpp"

const char *FONT_PATH = "./assets/JetBrainsMono-Regular.ttf";

AppInitCode App::init() {

    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return AppInitCode::Error;
    }

    // Create the window
    window = SDL_CreateWindow(
            "gtxt",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width,
            height,
            SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        return AppInitCode::Error;
    }

    // Create the renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cout << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        return AppInitCode::Error;
    }

    // Create FreeType library
    FT_Error error;
    error = FT_Init_FreeType(&library);
    if (error) {
        fprintf(stderr, "error: unable to load freetype library\n");
        return AppInitCode::Error;
    }

    // Create FreeType face
    error = FT_New_Face(library, FONT_PATH, 0, &face);
    if (error == FT_Err_Unknown_File_Format) {
        fprintf(stderr, "error: the font file could be opened and read,"
                        " but it appears that its font format is unsupported\n");
        return AppInitCode::Error;
    } else if (error) {
        fprintf(stderr, "error: another error code (%d) means that the font "
                        "file could not be opened or read, or that it is broken\n", error);
        return AppInitCode::Error;
    }

    return AppInitCode::Ok;
}

void App::deinit() {
    // Cleanup SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    // Cleanup FreeType
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}


void App::run() {
    uint fontsize = 24;
    bool display_cursor = false;
    SDL_AddTimer(333, [](Uint32 interval, void *param) {
        bool *boolean = static_cast<bool *>(param);
        *boolean = !*boolean;
        return interval;
    }, &display_cursor);

    bool running = true;
    while (running) {

        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                char key = event.key.keysym.sym;
                if (event.key.keysym.mod & KMOD_SHIFT) {

                    if (isalpha(key)) {
                        key = toupper(key);
                    } else {
                        switch (key) {
                            case '1':
                                key = '!';
                                break;
                            case '2':
                                key = '"';
                                break;
                            case '3':
                                key = '3';
                                break;
                            case '4':
                                key = '$';
                                break;
                            case '5':
                                key = '%';
                                break;
                            case '6':
                                key = '^';
                                break;
                            case '7':
                                key = '&';
                                break;
                            case '8':
                                key = '*';
                                break;
                            case '9':
                                key = '(';
                                break;
                            case '0':
                                key = ')';
                                break;
                            case '-':
                                key = '_';
                                break;
                            case '=':
                                key = '+';
                                break;
                            case ';':
                                key = ':';
                                break;
                            default:
                                break;
                        }
                    }
                } else if (event.key.keysym.mod & KMOD_CTRL) {
                    if (event.key.keysym.sym == 's') {
                        save_file();
                        exit(0);
                    }
                } else {
                    key = key;
                }

                int i;
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_A...SDL_SCANCODE_Z:
                    case SDL_SCANCODE_1...SDL_SCANCODE_0:
                    case SDL_SCANCODE_TAB...SDL_SCANCODE_SLASH:
                        content.insert(cursor_position, &key);
                        cursor_position++;
                        break;
                    case SDL_SCANCODE_RETURN:
                    case SDL_SCANCODE_RETURN2:
                        content.insert(cursor_position, sizeof(char), '\n');
                        cursor_position++;
                        break;
                    case SDL_SCANCODE_BACKSPACE:
                        if (content.length() != 0 && cursor_position != 0) {
                            content.erase(cursor_position - 1, 1);
                            cursor_position--;
                        }
                        break;
                    case SDL_SCANCODE_DELETE:
                        if (content.length() != 0)
                            content.erase(cursor_position, 1);
                        break;
                    case SDL_SCANCODE_RIGHT:
                        if (cursor_position != content.length())
                            cursor_position++;
                        break;
                    case SDL_SCANCODE_LEFT:
                        if (cursor_position != 0)
                            cursor_position--;
                        break;
                    case SDL_SCANCODE_UP:
                        // Start of current line
                        i = content.rfind('\n', cursor_position);
                        if (i == -1) {
                            cursor_position = 0;
                            break;
                        }

                        i = content.rfind('\n', i - 1);
                        if (i == -1) {
                            cursor_position = 0;
                            break;
                        }

                        cursor_position = i + 1;
                        break;
                    case SDL_SCANCODE_DOWN:
                        i = content.find('\n', cursor_position);
                        if (i == -1) {
                            cursor_position = content.length();
                        } else {
                            cursor_position = i + 1;
                        }
                        break;
                    default:
                        break;
                }
            }
        }

        // Fill background
        SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
        SDL_RenderClear(renderer);

        // Render text
        SDL_RenderDrawText(renderer, face, content, fontsize, 5, fontsize);

        // Render cursor
        if (display_cursor) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);


            // Calculate position of cursor.
            int offset_x = 0;
            int offset_y = 0;
            for (char c: content.substr(0, cursor_position)) {
                offset_x++;
                if (c == '\n') {
                    offset_x = 0;
                    offset_y++;
                }
            }

            // Scale by fontsize.
            offset_x *= (int) (fontsize / 1.6);
            offset_y *= (int) (fontsize);

            // Padding
            offset_y += (int) (fontsize / 6);

            SDL_Rect cursor_rect = {offset_x, offset_y, 2, (int) fontsize};
            SDL_RenderFillRect(renderer, &cursor_rect);
        }

        // Render the changes
        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(window);
    }
}

void App::load_file(std::string fp) {
    filepath = std::move(fp);

    std::ifstream file_stream(filepath);
    std::stringstream buffer;
    buffer << file_stream.rdbuf();
    content = buffer.str();
}

void App::save_file() {
    printf("saving content to file");
    std::ofstream file_stream(filepath);
    file_stream << content;
    file_stream.close();
}

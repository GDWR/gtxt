
#ifndef GRIDE_SDL_EXTS_HPP
#define GRIDE_SDL_EXTS_HPP

#include <SDL.h>
#include <ft2build.h>
#include "freetype/freetype.h"
#include <iostream>

void SDL_RenderDrawChar(SDL_Renderer *renderer, FT_Face face, char character, int fontsize, int x, int y);

void SDL_RenderDrawText(SDL_Renderer *renderer, FT_Face face, std::string text, int fontsize, int x, int y);

#endif

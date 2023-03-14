#include "sdl_exts.hpp"

void SDL_RenderDrawBitmap(SDL_Renderer *renderer, FT_Bitmap bitmap, int x, int y) {
    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(
      bitmap.buffer, bitmap.width, bitmap.rows, 8, bitmap.pitch, 0, 0, 0, 0xFF);

    if (surface == nullptr) {
        std::cout << "error: Couldn't make glyph surface - " << SDL_GetError() << std::endl;
        exit(-1);
    }

    SDL_Color colors[256];
    for (int i = 0; i < 256; i++) {
        colors[i].r = 200;
        colors[i].g = 200;
        colors[i].b = 236;
        colors[i].a = i;
    }
    SDL_SetPaletteColors(surface->format->palette, colors, 0, 256);
    SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);

    // Create a texture from the surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        SDL_FreeSurface(surface);
        std::cout << "error: Couldn't create texture from surface - " << SDL_GetError() << std::endl;
        exit(-1);
    }

    SDL_Rect dest = {x, y, (int) bitmap.width, (int) bitmap.rows};
    SDL_RenderCopy(renderer, texture, nullptr, &dest);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void SDL_RenderDrawChar(SDL_Renderer *renderer, FT_Face face, char character, int fontsize, int x, int y) {

    // Set the font size
    if (FT_Set_Pixel_Sizes(face, fontsize, fontsize)) {
        std::cout << "error: Couldn't set font size" << std::endl;
        exit(-1);
    }

    // Load character
    if (FT_Load_Char(face, character, FT_LOAD_RENDER)) {
        std::cout << "error: Couldn't render glyph" << std::endl;
        exit(-1);
    }

    int calculated_x = x + face->glyph->bitmap_left;
    int calculated_y = y - face->glyph->bitmap_top;
    SDL_RenderDrawBitmap(renderer, face->glyph->bitmap, calculated_x, calculated_y);
}

void SDL_RenderDrawText(SDL_Renderer *renderer, FT_Face face, std::string text, int fontsize, int x, int y) {
    uint offset_x = 0;
    uint offset_y = 0;
    uint offset_step = fontsize / 1.6;

    for (auto c : text) {
        switch (c) { // Handle some special cases like spaces and returns.
            case ' ':
                offset_x += offset_step;
                break;
            case '\n':
                offset_y += fontsize;
                offset_x = 0;
                break;
            default:
                SDL_RenderDrawChar(renderer, face, c, fontsize, x + offset_x, y + offset_y);
                offset_x += offset_step;
                break;
        }

    }
}

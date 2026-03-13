#pragma once

#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "SDL3_image/SDL_image.h"
#include "TextureAtlas.h"
#include <memory>
struct TextureAtlasLoader {

  static std::unique_ptr<TextureAtlas>
  loadAtlasFromFile(const char *pathToAtlas, SDL_Renderer *renderer,
                    const int screenWidth, const int screenHeight) {

    auto textureAtlas = std::make_unique<TextureAtlas>();
    auto surface = IMG_Load(pathToAtlas);

    auto convertedSurface =
        SDL_ConvertSurface(surface, SDL_PIXELFORMAT_ARGB8888);

    textureAtlas->pixels = static_cast<Uint32 *>(convertedSurface->pixels);
    textureAtlas->pitch = convertedSurface->pitch;
    textureAtlas->atlasWidth = convertedSurface->w;
    textureAtlas->atlasHeight = convertedSurface->h;
    textureAtlas->tilesPerRow =
        textureAtlas->atlasWidth / textureAtlas->tileWidth;
    textureAtlas->tilesPerColumn =
        textureAtlas->atlasHeight / textureAtlas->tileHeight;
    textureAtlas->texture = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
        screenWidth, screenHeight);

    return textureAtlas;
  }
};
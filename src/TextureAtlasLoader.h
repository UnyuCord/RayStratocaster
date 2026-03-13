#pragma once

#include "SDL3/SDL_pixels.h"
#include "SDL3_image/SDL_image.h"
#include "TextureAtlas.h"
#include <memory>
struct TextureAtlasLoader {

  static std::unique_ptr<TextureAtlas>
  loadAtlasFromFile(const char *pathToAtlas) {

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

    return textureAtlas;
  }
};
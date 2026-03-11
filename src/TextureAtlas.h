#pragma once

#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"
#include "SDL3/SDL_surface.h"
#include "SDL3_image/SDL_image.h"
struct TextureAtlas {

  int atlasWidth;
  int atlasHeight;

  const char *pathToAtlas;

  int tilesPerRow;
  int tilesPerColumn;

  int tileWidth;
  int tileHeight;

  SDL_Texture* texture;
  Uint32 *pixels;
  int pitch;

  TextureAtlas(const char *pathToAtlas, const int tileWidth,
               const int tileHeight)
      : pathToAtlas(pathToAtlas), tileWidth(tileWidth), tileHeight(tileHeight) {

    // TODO: Most likely very stupid, I dont care
    auto surface = IMG_Load(pathToAtlas);

    auto convertedSurface = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_ABGR8888);

    pixels = static_cast<Uint32 *>(convertedSurface->pixels);
    pitch = convertedSurface->pitch;

    atlasWidth = convertedSurface->w;
    atlasHeight = convertedSurface->h;

    tilesPerRow = atlasWidth / tileWidth;
    tilesPerColumn = atlasHeight / tileHeight;
  };
};
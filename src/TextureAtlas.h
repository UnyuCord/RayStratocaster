#pragma once

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"
struct TextureAtlas {

  int atlasWidth;
  int atlasHeight;

  const char *pathToAtlas;

  int tilesPerRow;
  int tilesPerColumn;

  // TODO: dont be hardcoded lol
  int tileWidth = 128;
  int tileHeight = 128;

  SDL_Texture *texture;
  Uint32 *pixels;
  int pitch;
};
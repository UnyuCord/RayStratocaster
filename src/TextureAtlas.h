#pragma once

#include "SDL3/SDL_stdinc.h"
struct TextureAtlas {

  const int DEFAULT_UNIFORM_TILE_DIMENSIONS = 64;

  int atlasWidth;
  int atlasHeight;

  const char *pathToAtlas;

  int tilesPerRow;
  int tilesPerColumn;

  // TODO: dont be hardcoded lol
  int tileWidth = DEFAULT_UNIFORM_TILE_DIMENSIONS;
  int tileHeight = DEFAULT_UNIFORM_TILE_DIMENSIONS;

  Uint32 *pixels;
  int pitch;
};
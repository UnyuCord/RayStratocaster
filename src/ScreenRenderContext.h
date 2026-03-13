#pragma once

#include "SDL3/SDL_render.h"
struct ScreenRenderContext {

  int screenWidth;
  int screenHeight;

  SDL_Texture *screenTexture;
  SDL_Renderer *renderer;

  void initTexture() {

    if (screenTexture != nullptr)
      return;

    screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                      SDL_TEXTUREACCESS_STREAMING, screenWidth,
                                      screenHeight);
  }

};
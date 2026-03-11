#pragma once

#include "SDL3/SDL_render.h"
#include "World.h"

class RayStratocaster {

public:
  // TODO: Variable screen size and also Renderer shouldnt be owner of these
  const int SCREEN_WIDTH = 1280;
  const int SCREEN_HEIGHT = 720;

  void renderPlayerView(const World &world, SDL_Renderer &renderer, SDL_Texture &texture);
  void renderTwoDimensionalView(const World &world, SDL_Renderer &renderer);
  RayStratocaster() = default;
};
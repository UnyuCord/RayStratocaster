#pragma once

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"
#include "World.h"

class RayStratocaster {

  Uint64 frameTime = 0Lu;
  Uint64 oldFrameTime = 0Lu;

public:
  // TODO: Variable screen size
  const int SCREEN_WIDTH = 640;
  const int SCREEN_HEIGHT = 480;

  void render(const World &world, SDL_Renderer &renderer);
  RayStratocaster() = default;
};
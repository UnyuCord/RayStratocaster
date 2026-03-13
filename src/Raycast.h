#pragma once

#include "SDL3/SDL_render.h"
#include "ScreenRenderContext.h"
#include "World.h"


// Shadow bonnie
class RayStratocaster {

public:

  void renderPlayerView(const World &world, const ScreenRenderContext &renderContext);
  void renderTwoDimensionalView(const World &world, SDL_Renderer &renderer);
  RayStratocaster() = default;
};
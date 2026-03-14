#pragma once

#include "SDL3/SDL_render.h"
#include "ScreenRenderContext.h"
#include "World.h"


// Shadow bonnie
class RayStratocaster {

  const int MAX_VIEW_DISTANCE = 20;

public:

  void renderPlayerView(const World &world, const ScreenRenderContext &renderContext);
  void renderTwoDimensionalView(const World &world, SDL_Renderer &renderer);
  RayStratocaster() = default;
};

struct Ray {

  Vector2D<double> direction;
  Vector2D<double> sideDistance;
  Vector2D<double> deltaDistance;
  
  double cameraX;

  int mapX;
  int mapY;

  int stepX;
  int stepY;

};
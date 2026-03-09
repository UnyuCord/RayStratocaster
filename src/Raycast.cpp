
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#define COLOR_RESET SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

constexpr int MAP_WIDTH = 24;
constexpr int MAP_HEIGHT = 24;
constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;

static SDL_Window *window = nullptr;
static SDL_Renderer *renderer = nullptr;

constexpr int MAP[MAP_WIDTH][MAP_HEIGHT] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

struct Vector2D {

  double x = 0;
  double y = 0;

  Vector2D() = default;

  Vector2D(double x, double y) : x(x), y(y) {}
};

int main(int argc, char *argv[]) {

  Vector2D positionVector = {22, 12};
  Vector2D directionVector = {-1, 0};
  Vector2D planeVector = {0, 0.66};

  auto done = false;

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    printf("Could not initialize video: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("RayStratocaster", SCREEN_WIDTH,
                                   SCREEN_HEIGHT, 0, &window, &renderer)) {
    printf("Could not create window or renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  double frameTime = 0;
  double oldFrameTime = 0;

  while (!done) {

    for (int x = 0; x < SCREEN_WIDTH; x++) {

      double cameraX = 2 * x / double(SCREEN_WIDTH) - 1;
      Vector2D rayDirectionVector = {
          directionVector.x + planeVector.x * cameraX,
          directionVector.y + planeVector.y * cameraX};

      auto mapX = static_cast<int>(positionVector.x);
      auto mapY = static_cast<int>(positionVector.y);

      Vector2D sideDistanceVector;
      Vector2D deltaDistanceVector = {std::abs(1 / rayDirectionVector.x),
                                      std::abs(1 / rayDirectionVector.y)};

      double perpendicularWallDistance;

      int stepX;
      int stepY;

      auto hit = false;
      int side;

      if (rayDirectionVector.x < 0) {
        stepX = -1;
        sideDistanceVector.x = (positionVector.x - mapX) * deltaDistanceVector.x;
      } else {
        stepX = 1;
        sideDistanceVector.x = (mapX + 1.0 - positionVector.x) * deltaDistanceVector.x;
      }

      if (rayDirectionVector.y < 0) {
        stepY = -1;
        sideDistanceVector.y = (positionVector.y - mapY) * deltaDistanceVector.y;
      } else {
        stepY = 1;
        sideDistanceVector.y = (mapY + 1.0 - positionVector.y) * deltaDistanceVector.y;
      }

      while (!hit) {

        if (sideDistanceVector.x < sideDistanceVector.y) {
          sideDistanceVector.x += deltaDistanceVector.x;
          mapX += stepX;
          side = 0;
        } else {
          sideDistanceVector.y += deltaDistanceVector.y;
          mapY += stepY;
          side = 1;
        }

        if (MAP[mapX][mapY] > 0)
          hit = true;
      }

      if (side == 0)
        perpendicularWallDistance = sideDistanceVector.x - deltaDistanceVector.x;
      else
        perpendicularWallDistance = sideDistanceVector.y - deltaDistanceVector.y;

      auto lineHeight =
          static_cast<int>(SCREEN_HEIGHT / perpendicularWallDistance);

      int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
      if (drawStart < 0)
        drawStart = 0;
      int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
      if (drawEnd >= SCREEN_HEIGHT)
        drawEnd = SCREEN_HEIGHT - 1;

      SDL_Color color;

      switch (MAP[mapX][mapY]) {
      case 1:
        color = {255, 0, 0, 255};
        break;
      case 2:
        color = {0, 255, 0, 255};
        break;
      case 3:
        color = {0, 0, 255, 255};
        break;
      case 4:
        color = {255, 255, 255, 255};
        break;
      default:
        color = {0, 255, 255, 255};
        break;
      }

      
      if (side) {
        color.r /= 2;
        color.g /= 2;
        color.b /= 2;
      }

      SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
      SDL_RenderLine(renderer, static_cast<float>(x),
                     static_cast<float>(drawStart), static_cast<float>(x),
                     static_cast<float>(drawEnd));
      COLOR_RESET
    }

    oldFrameTime = frameTime;
    frameTime = static_cast<double>(SDL_GetTicks());

    double currentFrameTime = (frameTime - oldFrameTime) / 1000.0;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    if (!SDL_RenderDebugTextFormat(renderer, 50, 50, "Frametime: %f",
                                   currentFrameTime)) {
      printf("Could not render text: %s\n", SDL_GetError());
      return SDL_APP_FAILURE;
    }
    SDL_RenderPresent(renderer);
    COLOR_RESET

    SDL_RenderClear(renderer);

    double moveSpeed = currentFrameTime * 5.0;
    double rotationSpeed = currentFrameTime * 3.0;

    if (SDL_Event event; SDL_PollEvent(&event)) {

      switch (event.type) {
      case SDL_EVENT_QUIT:
        done = true;
        break;
      }
    }
  }

  return SDL_APP_SUCCESS;
}

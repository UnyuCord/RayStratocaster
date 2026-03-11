#include "Raycast.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "World.h"
#include <SDL3/SDL.h>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

// TODO: Renderline function that accepts color
#define COLOR_RESET SDL_SetRenderDrawColor(&renderer, 0, 0, 0, 0);

void RayStratocaster::renderPlayerView(const World &world, SDL_Renderer &renderer) {

  const auto &position = world.getPlayer().position;
  const auto &direction = world.getPlayer().direction;
  const auto &plane = world.getPlayer().plane;

  for (int x = 0; x < SCREEN_WIDTH; x++) {

    double cameraX = 2 * x / double(SCREEN_WIDTH) - 1;
    Vector2D rayDirection = {direction.x + plane.x * cameraX,
                             direction.y + plane.y * cameraX};

    auto mapX = static_cast<int>(position.x);
    auto mapY = static_cast<int>(position.y);

    Vector2D<double> sideDistance;
    Vector2D deltaDistance = {std::abs(1 / rayDirection.x),
                              std::abs(1 / rayDirection.y)};

    double perpendicularWallDistance;

    int stepX;
    int stepY;

    auto hit = false;
    int side;

    if (rayDirection.x < 0) {
      stepX = -1;
      sideDistance.x = (position.x - mapX) * deltaDistance.x;
    } else {
      stepX = 1;
      sideDistance.x = (mapX + 1.0 - position.x) * deltaDistance.x;
    }

    if (rayDirection.y < 0) {
      stepY = -1;
      sideDistance.y = (position.y - mapY) * deltaDistance.y;
    } else {
      stepY = 1;
      sideDistance.y = (mapY + 1.0 - position.y) * deltaDistance.y;
    }

    while (!hit) {

      if (sideDistance.x < sideDistance.y) {
        sideDistance.x += deltaDistance.x;
        mapX += stepX;
        side = 0;
      } else {
        sideDistance.y += deltaDistance.y;
        mapY += stepY;
        side = 1;
      }

      if (world.MAP[mapX][mapY] > 0)
        hit = true;
    }

    if (side == 0)
      perpendicularWallDistance = sideDistance.x - deltaDistance.x;
    else
      perpendicularWallDistance = sideDistance.y - deltaDistance.y;

    auto lineHeight =
        static_cast<int>(SCREEN_HEIGHT / perpendicularWallDistance);

    int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
    if (drawStart < 0)
      drawStart = 0;
    int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
    if (drawEnd >= SCREEN_HEIGHT)
      drawEnd = SCREEN_HEIGHT - 1;

    SDL_Color color;

    // TODO: Use textures instead
    switch (world.MAP[mapX][mapY]) {
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

    SDL_SetRenderDrawColor(&renderer, color.r, color.g, color.b, color.a);
    SDL_RenderLine(&renderer, static_cast<float>(x),
                   static_cast<float>(drawStart), static_cast<float>(x),
                   static_cast<float>(drawEnd));
    COLOR_RESET
  }

  SDL_SetRenderDrawColor(&renderer, 255, 255, 255, 255);
  if (!SDL_RenderDebugTextFormat(&renderer, 50, 50, "FPS: %.2f",
                                 1.0 / world.getDeltaTime())) {
    throw std::runtime_error(SDL_GetError());
  }

  SDL_RenderDebugTextFormat(&renderer, 50, 60, "Player POS: x:%.2f y:%.2f",
                            position.x, position.y);
  SDL_RenderDebugTextFormat(&renderer, 50, 70, "Player DIR: x:%.2f y:%.2f",
                            direction.x, direction.y);
  SDL_RenderDebugTextFormat(&renderer, 50, 80, "Player PLN: x:%.2f y:%.2f",
                            plane.x, plane.y);

  SDL_RenderPresent(&renderer);
  COLOR_RESET

  SDL_RenderClear(&renderer);
};

// TODO: Need to refactor this badly, but it does work at least, if a little skewed at certain values...
void RayStratocaster::renderTwoDimensionalView(const World &world,
                                               SDL_Renderer &renderer) {

  const auto &position = world.getPlayer().position;
  const auto &direction = world.getPlayer().direction;
  const auto &plane = world.getPlayer().plane;

  SDL_FRect rects[MAP_WIDTH][MAP_HEIGHT];
  SDL_FRect playerRect;

  auto squareWidthScaledToResolution = float(SCREEN_WIDTH) / MAP_WIDTH;
  auto squareHeightScaledToResolution = float(SCREEN_HEIGHT) / MAP_HEIGHT;

  playerRect.h = 10;
  playerRect.w = 10;

  playerRect.x = float(position.x) * squareWidthScaledToResolution;
  playerRect.y = float(position.y) * squareHeightScaledToResolution;

  // doing this just so it kind of matches the "collision" with walls until i have actual collision boxes
  playerRect.x -= playerRect.w / 2;
  playerRect.y -= playerRect.h / 2;

  for (int x = 0; x < MAP_WIDTH; x++) {
    for (int y = 0; y < MAP_HEIGHT; y++) {

      SDL_Color color;
      switch (world.MAP[x][y]) {
      case 1:
        color = {255, 0, 0, SDL_ALPHA_OPAQUE};
        break;
      case 2:
        color = {0, 255, 0, SDL_ALPHA_OPAQUE};
        break;
      case 3:
        color = {0, 0, 255, SDL_ALPHA_OPAQUE};
        break;
      case 4:
        color = {255, 255, 255, SDL_ALPHA_OPAQUE};
        break;
      default:
        color = {0, 0, 0, SDL_ALPHA_OPAQUE};
        break;
      }

      SDL_SetRenderDrawColor(&renderer, color.r, color.g, color.b, color.a);
      rects[x][y].w = squareWidthScaledToResolution;
      rects[x][y].h = squareHeightScaledToResolution;
      rects[x][y].x = float(x) * squareWidthScaledToResolution;
      rects[x][y].y = float(y) * squareHeightScaledToResolution;

      SDL_RenderFillRect(&renderer, &rects[x][y]);
      COLOR_RESET
    }
  }

  SDL_SetRenderDrawColor(&renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(&renderer, &playerRect);
  COLOR_RESET

  // Render POS | POS + DIR
  SDL_SetRenderDrawColor(&renderer, 0, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderLine(
      &renderer, playerRect.x + (playerRect.w / 2),
      playerRect.y + (playerRect.h / 2),
      float((position.x + direction.x) * squareWidthScaledToResolution),
      float((position.y + direction.y) * squareHeightScaledToResolution));
  COLOR_RESET

  // Render POS | POS + DIR - PLANE
  SDL_SetRenderDrawColor(&renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderLine(&renderer, playerRect.x + (playerRect.w / 2),
                 playerRect.y + (playerRect.h / 2),
                 float((position.x + direction.x + plane.x) *
                       squareWidthScaledToResolution),
                 float((position.y + direction.y + plane.y) *
                       squareHeightScaledToResolution));

  // Render POS | POS + DIR + PLANE
  SDL_RenderLine(&renderer, playerRect.x + (playerRect.w / 2),
                 playerRect.y + (playerRect.h / 2),
                 float((position.x + direction.x - plane.x) *
                       squareWidthScaledToResolution),
                 float((position.y + direction.y - plane.y) *
                       squareHeightScaledToResolution));

  // Render POS + DIR - PLANE | POS + DIR + PLANE
  SDL_SetRenderDrawColor(&renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderLine(&renderer,
                 float((position.x + direction.x - plane.x) *
                       squareWidthScaledToResolution),
                 float((position.y + direction.y - plane.y) *
                       squareHeightScaledToResolution),
                 float((position.x + direction.x + plane.x) *
                       squareWidthScaledToResolution),
                 float((position.y + direction.y + plane.y) *
                       squareHeightScaledToResolution));

  COLOR_RESET

  SDL_RenderPresent(&renderer);
  SDL_RenderClear(&renderer);
}
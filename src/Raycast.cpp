#include "Raycast.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"
#include "World.h"
#include <SDL3/SDL.h>
#include <cmath>
#include <stdexcept>

// TODO: Renderline function that accepts color
#define COLOR_RESET SDL_SetRenderDrawColor(&renderer, 0, 0, 0, 0);

using namespace std;

void RayStratocaster::renderPlayerView(const World &world,
                                       SDL_Renderer &renderer,
                                       SDL_Texture &texture) {

  const auto &position = world.getPlayer().position;
  const auto &direction = world.getPlayer().direction;
  const auto &plane = world.getPlayer().plane;
  const auto &textureAtlas = world.textureAtlas;

  Uint32 screenBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];

  for (int x = 0; x < SCREEN_WIDTH; x++) {

    double cameraX = 2 * x / double(SCREEN_WIDTH) - 1;
    Vector2D rayDirection = {direction.x + plane.x * cameraX,
                             direction.y + plane.y * cameraX};

    auto mapX = static_cast<int>(position.x);
    auto mapY = static_cast<int>(position.y);

    Vector2D<double> sideDistance;
    Vector2D deltaDistance = {abs(1 / rayDirection.x), abs(1 / rayDirection.y)};

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

    int texIndex = world.MAP[mapX][mapY] - 1;

    int tileX = texIndex % textureAtlas.tilesPerRow;
    int tileY = texIndex / textureAtlas.tilesPerRow;

    double wallX;

    if (!side) {
      wallX = position.y + perpendicularWallDistance * rayDirection.y;
    } else {
      wallX = position.x + perpendicularWallDistance * rayDirection.x;
    }
    wallX -= floor(wallX);

    auto texX = int(wallX * double(textureAtlas.tileWidth));

    if (side == 0 && rayDirection.x > 0)
      texX = textureAtlas.tileWidth - texX - 1;
    if (side == 1 && rayDirection.y < 0)
      texX = textureAtlas.tileWidth - texX - 1;

    double step = 1.0 * textureAtlas.tileHeight / lineHeight;
    double texPos =
        (drawStart - (double)SCREEN_HEIGHT / 2 + (double)lineHeight / 2) * step;

    for (int y = drawStart; y < drawEnd; y++) {
      int texY = (int)texPos & (textureAtlas.tileHeight - 1);
      texPos += step;

      int atlasX = textureAtlas.tileWidth * tileX + texX;
      int atlasY = textureAtlas.tileHeight * tileY + texY;

      Uint32 color =
          textureAtlas.pixels[(atlasY * textureAtlas.pitch / 4) + atlasX];

      if(side == 1) color = ((color & 0xFF000000) | (color & 0x00FEFEFE) >> 1);

      screenBuffer[y * SCREEN_WIDTH + x] = color;
    }
  }

  SDL_UpdateTexture(&texture, nullptr, screenBuffer,
                    SCREEN_WIDTH * sizeof(Uint32));
  SDL_RenderTexture(&renderer, &texture, nullptr, nullptr);
  
  SDL_SetRenderDrawColor(&renderer, 255, 255, 255, 255);
  if (!SDL_RenderDebugTextFormat(&renderer, 50, 50, "FPS: %.2f",
                                 1.0 / world.getDeltaTime())) {
    throw runtime_error(SDL_GetError());
  }

  SDL_RenderDebugTextFormat(&renderer, 50, 60, "Player POS: x:%.2f y:%.2f",
                            position.x, position.y);
  SDL_RenderDebugTextFormat(&renderer, 50, 70, "Player DIR: x:%.2f y:%.2f",
                            direction.x, direction.y);
  SDL_RenderDebugTextFormat(&renderer, 50, 80, "Player PLN: x:%.2f y:%.2f",
                            plane.x, plane.y);

  COLOR_RESET

  // TODO: Remove this once floor and ceiling rendering works
  for (int z = 0; z < SCREEN_HEIGHT * SCREEN_WIDTH; z++)
    screenBuffer[z] = 0;

  SDL_RenderPresent(&renderer);
  SDL_RenderClear(&renderer);
};

// TODO: Need to refactor this badly, but it does work at least, if a little
// skewed at certain values...
// ALSO seemingly inverted??
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

  // doing this just so it kind of matches the "collision" with walls until i
  // have actual collision boxes
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
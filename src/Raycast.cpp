#include "Raycast.h"
#include "Player.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"
#include "ScreenRenderContext.h"
#include "World.h"
#include <SDL3/SDL.h>
#include <cmath>
#include <stdexcept>

// TODO: Renderline function that accepts color
#define COLOR_RESET_RCTX SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
#define COLOR_RESET SDL_SetRenderDrawColor(&renderer, 0, 0, 0, 0);

using namespace std;

namespace {

Ray setupRay(const Player &player, const int screenWidth, const int x) {

  Ray ray;

  ray.cameraX = 2 * x / (double)screenWidth - 1;
  ray.direction = {player.direction.x + player.plane.x * ray.cameraX,
                   player.direction.y + player.plane.y * ray.cameraX};

  ray.mapX = static_cast<int>(player.position.x);
  ray.mapY = static_cast<int>(player.position.y);

  Vector2D inverseRayDirection = {1.0 / ray.direction.x, 1.0 / ray.direction.y};

  ray.deltaDistance = {abs(inverseRayDirection.x), abs(inverseRayDirection.y)};

  if (ray.direction.x < 0) {
    ray.stepX = -1;
    ray.sideDistance.x = (player.position.x - ray.mapX) * ray.deltaDistance.x;
  } else {
    ray.stepX = 1;
    ray.sideDistance.x =
        (ray.mapX + 1.0 - player.position.x) * ray.deltaDistance.x;
  }

  if (ray.direction.y < 0) {
    ray.stepY = -1;
    ray.sideDistance.y = (player.position.y - ray.mapY) * ray.deltaDistance.y;
  } else {
    ray.stepY = 1;
    ray.sideDistance.y =
        (ray.mapY + 1.0 - player.position.y) * ray.deltaDistance.y;
  }

  return ray;
}

} // namespace

void RayStratocaster::renderPlayerView(
    const World &world, const ScreenRenderContext &renderContext) const {

  const auto &position = world.getPlayer().position;
  const auto &direction = world.getPlayer().direction;
  const auto &plane = world.getPlayer().plane;
  const auto &textureAtlas = world.getTextureAtlas();

  const auto &screenTexture = renderContext.screenTexture;
  const auto &renderer = renderContext.renderer;

  void *pixels;
  int pitch;

  SDL_LockTexture(screenTexture, nullptr, &pixels, &pitch);

  auto screenBuffer = static_cast<Uint32 *>(pixels);
  int screenPitch = pitch / sizeof(Uint32);

  for (int x = 0; x < renderContext.screenWidth; x++) {

    auto ray = setupRay(world.getPlayer(), renderContext.screenWidth, x);

    double perpendicularWallDistance;
    auto rayHasHitWall = false;
    int side;

    while (!rayHasHitWall) {

      if (ray.sideDistance.x < ray.sideDistance.y) {
        ray.sideDistance.x += ray.deltaDistance.x;
        ray.mapX += ray.stepX;
        side = 0;
      } else {
        ray.sideDistance.y += ray.deltaDistance.y;
        ray.mapY += ray.stepY;
        side = 1;
      }

      if (auto currentDistance = (side == 0)
                                     ? ray.sideDistance.x - ray.deltaDistance.x
                                     : ray.sideDistance.y - ray.deltaDistance.y;
          currentDistance > MAX_VIEW_DISTANCE_IN_MAP_TILES || ray.mapX < 0 || ray.mapY < 0 ||
          ray.mapX >= MAP_WIDTH || ray.mapY >= MAP_HEIGHT)
        break;

      if (world.MAP[ray.mapX][ray.mapY] > 0)
        rayHasHitWall = true;
    }

    if (!rayHasHitWall)
      continue;

    if (side == 0)
      perpendicularWallDistance = ray.sideDistance.x - ray.deltaDistance.x;
    else
      perpendicularWallDistance = ray.sideDistance.y - ray.deltaDistance.y;

    auto lineHeight = static_cast<int>(renderContext.screenHeight /
                                       perpendicularWallDistance);

    int drawStart = -lineHeight / 2 + renderContext.screenHeight / 2;
    if (drawStart < 0)
      drawStart = 0;
    int drawEnd = lineHeight / 2 + renderContext.screenHeight / 2;
    if (drawEnd >= renderContext.screenHeight)
      drawEnd = renderContext.screenHeight - 1;

    int texIndex = world.MAP[ray.mapX][ray.mapY] - 1;

    int tileX = texIndex % textureAtlas.tilesPerRow;
    int tileY = texIndex / textureAtlas.tilesPerRow;

    double wallX;

    if (!side) {
      wallX = position.y + perpendicularWallDistance * ray.direction.y;
    } else {
      wallX = position.x + perpendicularWallDistance * ray.direction.x;
    }
    wallX -= floor(wallX);

    auto texX = int(wallX * double(textureAtlas.tileWidth));

    if (side == 0 && ray.direction.x > 0)
      texX = textureAtlas.tileWidth - texX - 1;
    if (side == 1 && ray.direction.y < 0)
      texX = textureAtlas.tileWidth - texX - 1;

    double step = 1.0 * textureAtlas.tileHeight / lineHeight;
    double texPos = (drawStart - (double)renderContext.screenHeight / 2 +
                     (double)lineHeight / 2) *
                    step;

    auto *screenBufferColumn = screenBuffer + x;
    auto textureOffset =
        tileY * textureAtlas.tileHeight * textureAtlas.atlasWidth +
        tileX * textureAtlas.tileWidth + texX;
    auto const *textureColumn = &textureAtlas.pixels[textureOffset];
    auto shouldShade = (side == 1);

    for (int y = drawStart; y < drawEnd; y++) {
      int texY = (int)texPos & (textureAtlas.tileHeight - 1);
      texPos += step;

      auto color = textureColumn[texY * textureAtlas.atlasWidth];

      if (shouldShade)
        color = ((color & 0xFF000000) | (color & 0x00FEFEFE) >> 1);

      screenBufferColumn[y * screenPitch] = color;
    }
  }

  SDL_UnlockTexture(screenTexture);
  SDL_RenderTexture(renderer, screenTexture, nullptr, nullptr);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  if (!SDL_RenderDebugTextFormat(renderer, 50, 50, "FPS: %.2f",
                                 1.0 / world.getDeltaTime())) {
    throw runtime_error(SDL_GetError());
  }

  SDL_RenderDebugTextFormat(renderer, 50, 60, "Player POS: x:%.2f y:%.2f",
                            position.x, position.y);
  SDL_RenderDebugTextFormat(renderer, 50, 70, "Player DIR: x:%.2f y:%.2f",
                            direction.x, direction.y);
  SDL_RenderDebugTextFormat(renderer, 50, 80, "Player PLN: x:%.2f y:%.2f",
                            plane.x, plane.y);

  COLOR_RESET_RCTX

  for (int z = 0; z < renderContext.screenWidth * renderContext.screenHeight;
       z++) {
    screenBuffer[z] = 0;
  }
  SDL_RenderPresent(renderer);
  SDL_RenderClear(renderer);
};

// TODO: This is shit
void RayStratocaster::renderTwoDimensionalView(const World &world,
                                               SDL_Renderer &renderer) const {

  const auto &position = world.getPlayer().position;
  const auto &direction = world.getPlayer().direction;
  const auto &plane = world.getPlayer().plane;

  SDL_FRect rects[MAP_WIDTH][MAP_HEIGHT];
  SDL_FRect playerRect;

  auto squareWidthScaledToResolution = 1280.0F / MAP_WIDTH;
  auto squareHeightScaledToResolution = 720.0F / MAP_HEIGHT;

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
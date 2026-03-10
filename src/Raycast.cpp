#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

// TODO: Renderline function that accepts color
#define COLOR_RESET SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

constexpr int MAP_WIDTH = 24;
constexpr int MAP_HEIGHT = 24;
// TODO: Variable screen size
constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;

static SDL_Window *window = nullptr;
static SDL_Renderer *renderer = nullptr;

// TODO: Get map data from image
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

  // TODO: Create player singleton and move these values there
  Vector2D position = {22, 12};
  Vector2D direction = {-1, 0};
  Vector2D plane = {0, 0.66};

  auto done = false;

  // TODO: Have init function for all this startup stuff
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    printf("Could not initialize video: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // TODO: Also proper SDL error handling
  if (!SDL_CreateWindowAndRenderer("RayStratocaster", SCREEN_WIDTH,
                                   SCREEN_HEIGHT, 0, &window, &renderer)) {
    printf("Could not create window or renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // TODO: Make this an option
  SDL_SetRenderVSync(renderer, 1);

  auto frameTime = 0Lu;
  auto oldFrameTime = 0Lu;

  while (!done) {

    for (int x = 0; x < SCREEN_WIDTH; x++) {

      double cameraX = 2 * x / double(SCREEN_WIDTH) - 1;
      Vector2D rayDirection = {direction.x + plane.x * cameraX,
                               direction.y + plane.y * cameraX};

      auto mapX = static_cast<int>(position.x);
      auto mapY = static_cast<int>(position.y);

      Vector2D sideDistance;
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

        if (MAP[mapX][mapY] > 0)
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
    frameTime = SDL_GetTicks();

    double deltaTime = static_cast<double>(frameTime - oldFrameTime) / 1000.0;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    if (!SDL_RenderDebugTextFormat(renderer, 50, 50, "FPS: %.2f",
                                   1.0 / deltaTime)) {
      printf("Could not render text: %s", SDL_GetError());
      return SDL_APP_FAILURE;
    }
    SDL_RenderPresent(renderer);
    COLOR_RESET

    SDL_RenderClear(renderer);

    // TODO: Inputs for movement
    double moveSpeed = deltaTime * 5.0;
    double rotationSpeed = deltaTime * 3.0;

    if (SDL_Event event;
        SDL_PollEvent(&event) && event.type == SDL_EVENT_QUIT) {
      done = true;
    }
  }

  return SDL_APP_SUCCESS;
}

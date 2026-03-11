#include "Raycast.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "World.h"
#include <SDL3/SDL_main.h>
#include <stdexcept>

bool checkForQuitSignal() {
  if (SDL_Event event; SDL_PollEvent(&event) && event.type == SDL_EVENT_QUIT) {
    return true;
  } else
    return false;
}

int main(int argc, char *argv[]) {

  World world;
  auto done = false;

  RayStratocaster raycaster;

  SDL_Window *window;
  SDL_Renderer *renderer;

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    throw std::runtime_error(SDL_GetError());
  }

  // TODO: Also proper SDL error handling
  if (!SDL_CreateWindowAndRenderer("RayStratocaster", raycaster.SCREEN_WIDTH,
                                   raycaster.SCREEN_HEIGHT, 0, &window,
                                   &renderer)) {
    throw std::runtime_error(SDL_GetError());
  }

  // TODO: Make this an option
  SDL_SetRenderVSync(renderer, 1);

  while (!done) {
    raycaster.render(world, *renderer);
    world.updateWorld();
    done = checkForQuitSignal();
  }
  return 0;
}

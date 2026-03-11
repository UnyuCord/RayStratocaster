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

  SDL_Window *raycastWindow;
  SDL_Renderer *renderer;

  SDL_Window *twoDimensionalViewWindow;
  SDL_Renderer *twoDimensionalViewRenderer;

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    throw std::runtime_error(SDL_GetError());
  }

  // TODO: Also proper SDL error handling
  if (!SDL_CreateWindowAndRenderer("RayStratocaster", raycaster.SCREEN_WIDTH,
                                   raycaster.SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, &raycastWindow,
                                   &renderer)) {
    throw std::runtime_error(SDL_GetError());
  }

  if (!SDL_CreateWindowAndRenderer("RayStratocaster 2D Viewa", raycaster.SCREEN_WIDTH,
                                   raycaster.SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, &twoDimensionalViewWindow,
                                   &twoDimensionalViewRenderer)) {
    throw std::runtime_error(SDL_GetError());
  }
  
  // TODO: Make this an option
  SDL_SetRenderVSync(renderer, 1);
  SDL_SetRenderVSync(twoDimensionalViewRenderer, 1);

  while (!done) {
    raycaster.renderPlayerView(world, *renderer);
    // TODO: Make this view overlay with the raycast view when pressing a button i.e dont render in a seperate window
    raycaster.renderTwoDimensionalView(world, *twoDimensionalViewRenderer);
    world.updateWorld();
    done = checkForQuitSignal();
  }
  return 0;
}

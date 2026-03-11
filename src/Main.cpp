#include "Raycast.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"
#include "World.h"
#include <SDL3/SDL_main.h>
#include <stdexcept>

class Engine {

  double deltaTime;
  Uint64 currentTime;
  Uint64 oldTime;

  World world;
  bool done = false;

  RayStratocaster raycaster;

  SDL_Window *raycastWindow;
  SDL_Renderer *renderer;

  SDL_Window *twoDimensionalViewWindow;
  SDL_Renderer *twoDimensionalViewRenderer;

  bool checkForQuitSignal() const {
    if (SDL_Event event;
        SDL_PollEvent(&event) && event.type == SDL_EVENT_QUIT) {
      return true;
    } else
      return false;
  }

  void updateTime() {
    oldTime = currentTime;
    currentTime = SDL_GetPerformanceCounter();
    deltaTime = static_cast<double>(currentTime - oldTime) /
                static_cast<double>(SDL_GetPerformanceFrequency());
  }

public:
  bool isDone() const { return done; }

  void Tick() {
    updateTime();
    world.updateWorld(deltaTime);
    raycaster.renderPlayerView(world, *renderer);
    // TODO: Make this view overlay with the raycast view when pressing a button
    // i.e dont render in a seperate window
    raycaster.renderTwoDimensionalView(world, *twoDimensionalViewRenderer);
    done = checkForQuitSignal();
  }

  void initialize() {

    if (!SDL_Init(SDL_INIT_VIDEO)) {
      throw std::runtime_error(SDL_GetError());
    }

    // TODO: Also proper SDL error handling
    if (!SDL_CreateWindowAndRenderer(
            "RayStratocaster", raycaster.SCREEN_WIDTH, raycaster.SCREEN_HEIGHT,
            SDL_WINDOW_RESIZABLE, &raycastWindow, &renderer)) {
      throw std::runtime_error(SDL_GetError());
    }

    if (!SDL_CreateWindowAndRenderer(
            "RayStratocaster 2D View", raycaster.SCREEN_WIDTH,
            raycaster.SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE,
            &twoDimensionalViewWindow, &twoDimensionalViewRenderer)) {
      throw std::runtime_error(SDL_GetError());
    }

    // TODO: Make this an option
    SDL_SetRenderVSync(renderer, 1);
    SDL_SetRenderVSync(twoDimensionalViewRenderer, 1);

    SDL_SetRenderLogicalPresentation(renderer, raycaster.SCREEN_WIDTH,
                                     raycaster.SCREEN_HEIGHT,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderLogicalPresentation(
        twoDimensionalViewRenderer, raycaster.SCREEN_WIDTH,
        raycaster.SCREEN_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
  }

  ~Engine() {

    // TODO: dunno if i should even do it this way

    SDL_DestroyRenderer(renderer);
    SDL_DestroyRenderer(twoDimensionalViewRenderer);

    SDL_DestroyWindow(raycastWindow);
    SDL_DestroyWindow(twoDimensionalViewWindow);

    SDL_QuitSubSystem(SDL_INIT_VIDEO);

    SDL_Quit();
  }
};

int main(int argc, char *argv[]) {

  Engine engine;

  engine.initialize();

  while (!engine.isDone()) {
    engine.Tick();
  }

  return 0;
}

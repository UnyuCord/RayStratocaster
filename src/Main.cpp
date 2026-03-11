#include "Raycast.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_version.h"
#include "SDL3/SDL_video.h"
#include "World.h"
#include <SDL3/SDL_main.h>
#include <cstring>
#include <stdexcept>

#define SDL_INFO_LOG_PREFIX "[RayStratocaster] "

class Engine {

  const char *RAYCAST_WINDOW_NAME = "RayStratocaster";
  const char *TWO_DIMENSIONAL_VIEW_WINDOW_NAME = "RayStratocaster 2D View";

  double deltaTime;
  Uint64 currentTime;
  Uint64 oldTime;

  World world;
  bool done = false;
  bool render2D = false;

  RayStratocaster raycaster;

  SDL_Window *raycastWindow;
  SDL_Renderer *renderer;
  SDL_Texture *texture;

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
    // TODO: should take deltatime directly from engine i think so pass it here
    raycaster.renderPlayerView(world, *renderer, *texture);
    // TODO: Make this view overlay with the raycast view when pressing a button
    // i.e dont render in a seperate window
    if (render2D)
      raycaster.renderTwoDimensionalView(world, *twoDimensionalViewRenderer);
    done = checkForQuitSignal();
  }

  void initialize(bool render2DArg) {

    render2D = render2DArg;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
      throw std::runtime_error(SDL_GetError());
    }

    SDL_Log("Initialized SDL: v%i", SDL_VERSION);

    // TODO: Better error handling
    if (!SDL_CreateWindowAndRenderer(
            RAYCAST_WINDOW_NAME, raycaster.SCREEN_WIDTH,
            raycaster.SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, &raycastWindow,
            &renderer)) {
      throw std::runtime_error(SDL_GetError());
    } else {

      SDL_SetRenderVSync(renderer, 1);
      SDL_SetRenderLogicalPresentation(renderer, raycaster.SCREEN_WIDTH,
                                       raycaster.SCREEN_HEIGHT,
                                       SDL_LOGICAL_PRESENTATION_LETTERBOX);

      SDL_Log("Set up window %s and its renderer", RAYCAST_WINDOW_NAME);
    }

    // TODO: meh
    if (render2D) {

      if (!SDL_CreateWindowAndRenderer(
              TWO_DIMENSIONAL_VIEW_WINDOW_NAME, raycaster.SCREEN_WIDTH,
              raycaster.SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE,
              &twoDimensionalViewWindow, &twoDimensionalViewRenderer)) {
        throw std::runtime_error(SDL_GetError());
      } else {

        SDL_SetRenderLogicalPresentation(
            twoDimensionalViewRenderer, raycaster.SCREEN_WIDTH,
            raycaster.SCREEN_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
        SDL_SetRenderVSync(twoDimensionalViewRenderer, 1);
      }

      SDL_Log("Set up window %s and its renderer",
              TWO_DIMENSIONAL_VIEW_WINDOW_NAME);
    }

    SDL_Log("Creating texture...");
    texture = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
        raycaster.SCREEN_WIDTH, raycaster.SCREEN_HEIGHT);

    SDL_Log("Init complete!");
  }

  ~Engine() {

    // TODO: dunno if i should even do it this way

    SDL_DestroyRenderer(renderer);
    SDL_DestroyRenderer(twoDimensionalViewRenderer);

    SDL_DestroyWindow(raycastWindow);
    SDL_DestroyWindow(twoDimensionalViewWindow);

    SDL_QuitSubSystem(SDL_INIT_VIDEO);

    SDL_Log("Destroyed SDL things, quitting...");

    SDL_Quit();
  }
};

int main(int argc, char *argv[]) {

  SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_INFO, SDL_INFO_LOG_PREFIX);

  auto render2D = false;

  // TODO: Proper arg passing and resolution
  if (argc > 1 && strcmp(argv[1], "--render2D") == 0) {
    render2D = true;
    SDL_Log("Rendering 2D View is enabled");
  }

  Engine engine;

  engine.initialize(render2D);

  SDL_Log("Beginning engine tick loop...");
  while (!engine.isDone()) {
    engine.Tick();
  }
  SDL_Log("Engine tick loop stopped!");

  return 0;
}

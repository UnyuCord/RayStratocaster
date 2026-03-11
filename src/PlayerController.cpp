#include "PlayerController.h"
#include "SDL3/SDL_scancode.h"
#include "World.h"

void PlayerController::handleInput(const bool *keyboardState, World &world) const {

  // TODO: make it possible to rebind keys

  Player& player = world.getPlayer();

  if (keyboardState[SDL_SCANCODE_UP]) {
    player.moveUp(world);
  }

  if (keyboardState[SDL_SCANCODE_DOWN]) {
    player.moveDown(world);
  }

  if (keyboardState[SDL_SCANCODE_LEFT]) {
    player.moveLeft(world);
  }

  if (keyboardState[SDL_SCANCODE_RIGHT]) {
    player.moveRight(world);
  }
}
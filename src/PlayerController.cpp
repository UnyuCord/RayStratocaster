#include "PlayerController.h"
#include "SDL3/SDL_scancode.h"
#include "World.h"

// TODO: A controller for the player should be its own entity, in the future there might be multiple types of controllers
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

  if (keyboardState[SDL_SCANCODE_F1]) {
    player.resetPosition();
  }
  
}
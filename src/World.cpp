#include "World.h"
#include "SDL3/SDL_keyboard.h"

double World::getDeltaTime() const { return deltaTime; }

Player &World::getPlayer() { return player; }
const Player &World::getPlayer() const { return player; }

void World::updateWorld(const double engineDeltaTime) {

  this->deltaTime = engineDeltaTime;
  // TODO: Passing a reference of world here seems meh, maybe just pass map data
  // and deltatime here in the future?
  playerController.handleInput(SDL_GetKeyboardState(nullptr), *this);
}
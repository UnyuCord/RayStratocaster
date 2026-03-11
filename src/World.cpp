#include "World.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_timer.h"

double World::getDeltaTime() const { return deltaTime / 1000.0; }

Player& World::getPlayer() { return player; }
const Player& World::getPlayer() const { return player; }

void World::updateTime() {
  oldTime = currentTime;
  currentTime = SDL_GetPerformanceCounter();
  deltaTime = static_cast<double>(currentTime - oldTime) * 1000 /
              static_cast<double>(SDL_GetPerformanceFrequency());
}

void World::updateWorld() { 

    updateTime(); 
    // TODO: Passing a reference of world here seems meh, maybe just pass map data and deltatime here in the future?
    playerController.handleInput(SDL_GetKeyboardState(nullptr), *this);

}
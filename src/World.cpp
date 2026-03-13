#include "World.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_render.h"
#include "TextureAtlas.h"
#include "TextureAtlasLoader.h"
#include <memory>

double World::getDeltaTime() const { return deltaTime; }

Player &World::getPlayer() { return player; }
const Player &World::getPlayer() const { return player; }

const TextureAtlas &World::getTextureAtlas() const { return *textureAtlas; }

void World::initializeWorld(SDL_Renderer *renderer, const int screenHeight,
                            const int screenWidth) {

  textureAtlas = TextureAtlasLoader::loadAtlasFromFile(
      "resources/textures/tex.png", renderer, screenHeight, screenWidth);
}

void World::updateWorld(const double engineDeltaTime) {

  this->deltaTime = engineDeltaTime;
  // TODO: Passing a reference of world here seems meh, maybe just pass map data
  // and deltatime here in the future?
  playerController.handleInput(SDL_GetKeyboardState(nullptr), *this);
}
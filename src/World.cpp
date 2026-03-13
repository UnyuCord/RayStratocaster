#include "World.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "TextureAtlas.h"
#include "TextureAtlasLoader.h"
#include <memory>

double World::getDeltaTime() const { return deltaTime; }

Player &World::getPlayer() { return player; }
const Player &World::getPlayer() const { return player; }

const TextureAtlas &World::getTextureAtlas() const { return *textureAtlas; }

SDL_Texture &World::getScreenTexture() { return *screenTexture; }

void World::initializeWorld(SDL_Renderer *renderer, const int screenWidth,
                            const int screenHeight) {

  textureAtlas =
      TextureAtlasLoader::loadAtlasFromFile("resources/textures/tex.png");

  screenTexture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                        SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);
}

void World::updateWorld(const double engineDeltaTime) {

  this->deltaTime = engineDeltaTime;
  // TODO: Passing a reference of world here seems meh, maybe just pass map data
  // and deltatime here in the future?
  playerController.handleInput(SDL_GetKeyboardState(nullptr), *this);
}
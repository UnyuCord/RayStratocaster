#include "World.h"
#include "TextureAtlas.h"
#include "TextureAtlasLoader.h"
#include <memory>

double World::getDeltaTime() const { return deltaTime; }

Player &World::getPlayer() { return player; }
const Player &World::getPlayer() const { return player; }

const TextureAtlas &World::getTextureAtlas() const { return *textureAtlas; }

void World::initializeWorld() {

  textureAtlas =
      TextureAtlasLoader::loadAtlasFromFile("../resources/textures/default.png");
}

void World::updateWorld(const double engineDeltaTime) {

  this->deltaTime = engineDeltaTime;
}
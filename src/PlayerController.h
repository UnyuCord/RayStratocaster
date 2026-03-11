#pragma once

class World;

class PlayerController {

public:
  void handleInput(const bool *keyboardState, World &world) const;
};
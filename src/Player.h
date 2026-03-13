#pragma once

#include "Vector2D.h"

class World;

struct Player {

  // TODO: base position should be 0,0 in the future
  Vector2D<double> position = {22, 12};
  Vector2D<double> direction = {-1, 0};
  Vector2D<double> plane = {0, 0.66};

  const int baseHealth = 100;
  // sqrs/s
  const double baseMoveSpeed = 5.0;
  // rad/s
  const double baseTurnSpeed = 3.0;

  int currentHealth = baseHealth;
  double currentMoveSpeed = baseMoveSpeed;
  double currentTurnSpeed = baseTurnSpeed;

  void moveUp(const World &world);
  void moveDown(const World &world);
  void moveRight(const World &world);
  void moveLeft(const World &world);
  void resetPosition();

  Player() = default;
  explicit Player(Vector2D<double> position) : position(position) {};
};
#pragma once

#include "Vector2D.h"
struct Player {

  Vector2D<double> position = {0, 0};
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

  Player() = default;
  explicit Player(Vector2D<double> position) : position(position) {};
};
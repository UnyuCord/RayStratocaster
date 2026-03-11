#include "Player.h"
#include "World.h"
#include <cmath>

// TODO: Player likes to get stuck on edges, probably because of world map being an int while actual position being double

void Player::moveUp(const World &world) {
  if (world.MAP[int(position.x + direction.x * (currentMoveSpeed * world.getDeltaTime()))]
               [int(position.y)] == false)
    position.x += direction.x * (currentMoveSpeed * world.getDeltaTime());
  if (world.MAP[int(position.x)]
               [int(position.y + direction.y * currentMoveSpeed * world.getDeltaTime())] == false)
    position.y += direction.y * (currentMoveSpeed * world.getDeltaTime());
}

void Player::moveDown(const World &world) {
  if (world.MAP[int(position.x + direction.x * currentMoveSpeed * world.getDeltaTime())]
               [int(position.y)] == false)
    position.x -= direction.x * (currentMoveSpeed * world.getDeltaTime());
  if (world.MAP[int(position.x)]
               [int(position.y + direction.y * currentMoveSpeed * world.getDeltaTime())] == false)
    position.y -= direction.y * (currentMoveSpeed * world.getDeltaTime());
}

void Player::moveRight(const World &world) {

  double oldDirX = direction.x;
  direction.x = direction.x * cos(-currentTurnSpeed * world.getDeltaTime()) -
                direction.y * sin(-currentTurnSpeed * world.getDeltaTime());
  direction.y = oldDirX * sin(-currentTurnSpeed * world.getDeltaTime()) +
                direction.y * cos(-currentTurnSpeed * world.getDeltaTime());
  double oldPlaneX = plane.x;
  plane.x = plane.x * cos(-currentTurnSpeed * world.getDeltaTime()) -
            plane.y * sin(-currentTurnSpeed * world.getDeltaTime());
  plane.y = oldPlaneX * sin(-currentTurnSpeed * world.getDeltaTime()) +
            plane.y * cos(-currentTurnSpeed * world.getDeltaTime());
}

void Player::moveLeft(const World &world) {

  double oldDirX = direction.x;
  direction.x = direction.x * cos(currentTurnSpeed * world.getDeltaTime()) -
                direction.y * sin(currentTurnSpeed * world.getDeltaTime());
  direction.y = oldDirX * sin(currentTurnSpeed * world.getDeltaTime()) +
                direction.y * cos(currentTurnSpeed * world.getDeltaTime());
  double oldPlaneX = plane.x;
  plane.x = plane.x * cos(currentTurnSpeed * world.getDeltaTime()) -
            plane.y * sin(currentTurnSpeed * world.getDeltaTime());
  plane.y = oldPlaneX * sin(currentTurnSpeed * world.getDeltaTime()) +
            plane.y * cos(currentTurnSpeed * world.getDeltaTime());
}
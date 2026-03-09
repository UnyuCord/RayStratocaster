
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <X11/Xlibint.h>

constexpr int MAP_WIDTH = 24;
constexpr int MAP_HEIGHT = 24;
constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;


constexpr int MAP[MAP_WIDTH][MAP_HEIGHT] =
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

struct Vector2D {
    
    double x = 0;
    double y = 0;

    Vector2D() = default;

    Vector2D(double x, double y) {
        this->x = x;
        this->y = y;
    }

};

int main(int argc, char *argv[]) {

    Vector2D positionVector = {22, 12};
    Vector2D directionVector = {-1, 0};
    Vector2D planeVector = {0, 0.66};

    auto *window = SDL_CreateWindow("RayStratocaster", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    auto *renderer = SDL_CreateRenderer(window, "RayStratocasterRenderer");
    

    double frameTime = 0;
    double oldFrameTime = 0;

    while(true) {
        
        for (int x = 0; x < SCREEN_WIDTH; x++) {

            double cameraX = 2 * x / double(SCREEN_WIDTH) - 1;
            Vector2D rayVector = {
                directionVector.x + planeVector.x * cameraX,
                directionVector.y + planeVector.y * cameraX
            };

            auto mapX = static_cast<int>(positionVector.x);
            auto mapY = static_cast<int>(positionVector.y);

            Vector2D sideDistanceVector;
            Vector2D deltaDistanceVector = {
                std::abs(1 / rayVector.x),
                std::abs(1 / rayVector.y)
            };

            double perpendicularWallDistance;

            Vector2D stepVector;

            auto hit = false;
            int side;


            if (rayVector.x < 0) {
                stepVector.x = -1;
                sideDistanceVector.x = (positionVector.x - mapX) * deltaDistanceVector.x;
            } else {
                stepVector.x = 1;
                sideDistanceVector.x = (mapX + 1 - positionVector.x) * deltaDistanceVector.x;
            }

            if (rayVector.y < 0) {
                stepVector.y = -1;
                sideDistanceVector.y = (positionVector.y - mapY) * deltaDistanceVector.y;
            } else {
                stepVector.y = 1;
                sideDistanceVector.y = (mapY + 1 - positionVector.y) * deltaDistanceVector.y;
            }

            while (!hit) {

                if (sideDistanceVector.x < sideDistanceVector.y) {
                    sideDistanceVector.x += deltaDistanceVector.x;
                    mapX += int(stepVector.x);
                    side = 0;
                } else {
                    sideDistanceVector.y += deltaDistanceVector.y;
                    mapY += stepVector.y;
                    side = 1;
                }

                if (MAP[mapX][mapY] > 0) hit = true;

            }

            if (side == 0) perpendicularWallDistance = sideDistanceVector.x - deltaDistanceVector.y;
            else           perpendicularWallDistance = sideDistanceVector.y - deltaDistanceVector.y;

            int lineHeight = SCREEN_HEIGHT / perpendicularWallDistance;

            int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
            if (drawStart < 0) drawStart = 0;
            int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
            if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

            SDL_Color color;

            switch(MAP[mapX][mapY]) {
                case 1: color = {255, 0, 0, 255}; break;
                case 2: color = {0, 255, 0, 255}; break;
                case 3: color = {0, 0, 255, 255}; break;
                case 4: color = {255, 255, 255, 255}; break;
                default: color = {0, 255, 255, 255}; break;
            }

            // TODO: Darker edges
        }
    }


    return 0;
}
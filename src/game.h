#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "enemy.h"
#include <thread>
#include <chrono>
#include <future>
#include <algorithm>
#include "barrier.h"

class Enemy;

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  void fight(Snake &snake, Enemy &enemy);
  std::vector<std::future<void>> futures;


private:
  Snake snake;
  Enemy enemy;
  SDL_Point food;
  std::shared_ptr<Barrier> barrier;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();
  void Update();
  int grid_width_;
  int grid_height_;
};

#endif
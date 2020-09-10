#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"
#include <memory>
#include "barrier.h"
#include <thread>

class Barrier;

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {}

  void Update(std::shared_ptr<Barrier> barr);

  void GrowBody();
  bool SnakeCell(int x, int y);

  Direction direction = Direction::kUp;

  float speed{0.2f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;
  void Life();
  int get_Life() { return Life_num; }
  void plus_Life() { Life_num += 1; }

private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell, std::shared_ptr<Barrier> barr);
  int Life_num{30};
  int count_hold{0};
  int turn;

  bool growing{false};
  int grid_width;
  int grid_height;
};

#endif
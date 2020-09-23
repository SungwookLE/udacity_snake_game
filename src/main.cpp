#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

std::mutex Enemy::mtx;

int main() {
  int num_of_enemies = 10;

  
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{800};
  constexpr std::size_t kScreenHeight{800};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};
  
  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  
  //std::cout << "Number of Enemies: ";
  //std::cin >> num_of_enemies;

  Game game(kGridWidth, kGridHeight, num_of_enemies);
  game.Run(controller, renderer, kMsPerFrame);

  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}
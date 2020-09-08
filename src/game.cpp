#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      enemy(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width) - 1),
      random_h(0, static_cast<int>(grid_height) - 1),
      grid_width_(grid_width),
      grid_height_(grid_height)
      {
      barrier = std::make_shared<Barrier>(grid_width, grid_height);
      PlaceFood();
      }

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food, barrier, enemy);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    //std::cout <<"(grid_width,grid_heigth): "<< grid_width<< ", "<< grid_heigth <<" (x,y): "<< x <<", "<< y << std::endl;
    // Check that the location is not occupied by a snake item before placing
    // food.
    if ( (!snake.SnakeCell(x, y)) && (!barrier->BarrierCell(x,y)) )  {
      food.x = x;
      food.y = y; 
      return;
    }
  }
}

void Game::Update() {
 
  int test = 0;
  if (!snake.alive)
    return;

  /*
  for (int i = 0; i < 2; ++i){
    Enemy enem(grid_width_, grid_height_);
    futures.emplace_back( std::async(std::launch::async, &Enemy::psuhBack, &enemy, std::move(enem)) );
  }
  std::for_each(futures.begin(), futures.end(), [](std::future<void> &ftr) { ftr.wait(); });
  // 음 이게 아니라 각각의 에너미 객체를 일단 만들어 놓고 이 에너미 객체들의 각각의 행동들을 멀티 쓰레드로 가게 만들어 두면 되겠다. (9/8)
  
  */
 
  snake.Update(barrier);
  enemy.Update(barrier);

    // std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    // std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    // std::chrono::nanoseconds nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    // std::cout << nsec.count() << " ns" << std::endl;

    int new_x = static_cast<int>(snake.head_x);
    int new_y = static_cast<int>(snake.head_y);

    int new_enemy_x = static_cast<int>(enemy.head_x);
    int new_enemy_y = static_cast<int>(enemy.head_y);
   
    enemy.FoodSearch(food, barrier);
    fight(snake, enemy);
    snake.Life();
    enemy.ReStart();

    // Check if there's food over here
    if (food.x == new_x && food.y == new_y)
    {
      score++;
      barrier->Update(score);
      PlaceFood();
      // Grow snake and increase speed.
      snake.GrowBody();
      snake.speed += 0.02;
  }
  else if (food.x == new_enemy_x && food.y == new_enemy_y){
    PlaceFood();
    enemy.GrowBody();
    enemy.speed += 0.01;
  }

}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }

void Game::fight(Snake& snake, Enemy& enemy){
  SDL_Point snake_head{
      static_cast<int>(snake.head_x),
      static_cast<int>(snake.head_y)};

  SDL_Point enemy_head{
      static_cast<int>(enemy.head_x),
      static_cast<int>(enemy.head_y)};
  
    if (snake_head.x == static_cast<int>(enemy.body.begin()->x) && snake_head.y == static_cast<int>(enemy.body.begin()->y) &&enemy.size >1){
      enemy.alive = false;
      snake.GrowBody();
      snake.speed += 0.02;
    }

    if (enemy_head.x == static_cast<int>(snake.body.begin()->x) && enemy_head.y == static_cast<int>(snake.body.begin()->y) && snake.size >1){
      snake.alive = false;
      enemy.GrowBody();
      enemy.speed += 0.01;
    }
}



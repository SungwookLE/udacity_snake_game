#include "game.h"
#include <iostream>

Game::Game(std::size_t grid_width, std::size_t grid_height, int num_of_enemy)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width) - 1),
      random_h(0, static_cast<int>(grid_height) - 1),
      _num_of_enemy(num_of_enemy)
      {
        for (int i = 0; i < num_of_enemy; ++i){
          enemies.push_back(std::make_shared<Enemy>(grid_width, grid_height, i+1));
        }
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
    //std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    controller.HandleInput(running, snake);
    Update(running);
    renderer.Render(snake, food, barrier, enemies ,_num_of_enemy, static_cast<Renderer::kind_of_food>(kind_of_food_));
    //std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    frame_end = SDL_GetTicks();
    //std::chrono::nanoseconds msec = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    //std::cout << msec.count() << " ms" << std::endl;
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

  SDL_Delay(500);
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);

    if (x%8 ==0 )
      kind_of_food_ = kind_of_food::double_up;
    else if (x%8 == 1 )
      kind_of_food_ = kind_of_food::kill;
    else if (x%8 == 2)
      kind_of_food_ = kind_of_food::life_up;
    else
      kind_of_food_ = kind_of_food::normal;

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

void Game::Update(bool &runnig) {
 
  int test = 0;
  if (snake.get_Life() < 0 ){
    std::cout << "Game OVER!" << std::endl;
    runnig = false;
  }
  /*
  for (int i = 0; i < 2; ++i){
    Enemy enem(grid_width_, grid_height_);
    futures.emplace_back( std::async(std::launch::async, &Enemy::psuhBack, &enemy, std::move(enem)) );
  }
  std::for_each(futures.begin(), futures.end(), [](std::future<void> &ftr) { ftr.wait(); });
  // 음 이게 아니라 각각의 에너미 객체를 일단 만들어 놓고 이 에너미 객체들의 각각의 행동들을 멀티 쓰레드로 가게 만들어 두면 되겠다. (9/8)
  // std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
  // std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
  // std::chrono::nanoseconds nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
  // std::cout << nsec.count() << " ns" << std::endl;
  */
  for (int i = 0; i < _num_of_enemy; ++i){
    std::shared_ptr<Enemy> temp_enemy = enemies.at(i);
    futures.emplace_back(std::async(std::launch::async, &Enemy::Update, temp_enemy, barrier));
  }
  snake.Update(barrier);
  std::for_each(futures.begin(), futures.end(), [](std::future<void> &ftr) { ftr.wait(); });

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  
  futures.clear();
  for (int i = 0; i < _num_of_enemy; ++i)
  {
    std::shared_ptr<Enemy> temp_enemy = enemies.at(i);
    futures.emplace_back(std::async(std::launch::async, &Enemy::FoodSearch, temp_enemy, food,barrier));
  }
  std::for_each(futures.begin(), futures.end(), [](std::future<void> &ftr) { ftr.wait(); });
  
  fight(snake, enemies, _num_of_enemy);
  snake.Life();

  futures.clear();
  for (int i = 0; i < _num_of_enemy; ++i)
  {
    std::shared_ptr<Enemy> temp_enemy = enemies.at(i);
    futures.emplace_back(std::async(std::launch::async, &Enemy::ReStart, temp_enemy));
  }
  std::for_each(futures.begin(), futures.end(), [](std::future<void> &ftr) { ftr.wait(); });

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y){
    score+=2;
    barrier->Update(score);

    if (kind_of_food_ == kind_of_food::kill){
      std::cout << "Kill anyone Bigger than ME!!!\n";
      futures.clear();
      for (int i = 0; i < _num_of_enemy; ++i){
        std::shared_ptr<Enemy> temp_enemy = enemies.at(i);
        futures.emplace_back(std::async(std::launch::async, &Enemy::kill_cond, temp_enemy, snake.size));
      }
      std::for_each(futures.begin(), futures.end(), [](std::future<void> &ftr) { ftr.wait(); });
    }
    else if  (kind_of_food_ == kind_of_food::double_up){
      std::cout << "Bonus Score!\n";
      score+=1;
      snake.speed += 0.1;
    }
    else if ( kind_of_food_ == kind_of_food::life_up){
      snake.plus_Life();
      
      std::cout << "Life Up:"
                << " Life is Remained: " << snake.get_Life() << "[ea]! " << std::endl;
    }

    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.1;

    std::cout << "Snake(me) is growing! Score: " << score << std::endl;
    
  }
  else{
    for (int i = 0; i < _num_of_enemy; ++i){
      int new_enemy_x = static_cast<int>(enemies.at(i)->head_x);
      int new_enemy_y = static_cast<int>(enemies.at(i)->head_y);
      if (food.x == new_enemy_x && food.y == new_enemy_y){
        
        enemies.at(i)->enemy_score += 1;
        barrier->Update(enemies.at(i)->enemy_score);
        if (kind_of_food_ == kind_of_food::double_up){
          enemies.at(i)->speed += 0.02;
        }
        PlaceFood();
        enemies.at(i)->GrowBody();
        enemies.at(i)->speed += 0.02;
        std::cout << "Enemy snake #" << enemies.at(i)->getID() << " is growing!\n";
        }
      }
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }

void Game::fight(Snake& snake, std::vector<std::shared_ptr<Enemy>> enemy, int num_of_enemy){
  SDL_Point snake_head{
      static_cast<int>(snake.head_x),
      static_cast<int>(snake.head_y)};

  for (int i = 0; i < num_of_enemy; ++i){

    SDL_Point enemy_head{
        static_cast<int>(enemy.at(i)->head_x),
        static_cast<int>(enemy.at(i)->head_y)};

    if (snake_head.x == static_cast<int>(enemy.at(i)->body.begin()->x) && snake_head.y == static_cast<int>(enemy.at(i)->body.begin()->y) && enemy.at(i)->size > 1)
    {
      enemy.at(i)->alive = false;
      snake.GrowBody();
      snake.speed += 0.03;
      score+=1;
      std::cout << "Snake(me) kill the Enemy snake #" << enemy.at(i)->getID() << "! Score: "<< score<< std::endl;
    }

    if (enemy_head.x == static_cast<int>(snake.body.begin()->x) && enemy_head.y == static_cast<int>(snake.body.begin()->y) && snake.size >1){
      snake.alive = false;
      enemy.at(i)->GrowBody();
      enemy.at(i)->speed += 0.01;
      std::cout << "Enemy snake #" << enemy.at(i)->getID() << " kills Snake(you)!" << std::endl;
    }
  }
}



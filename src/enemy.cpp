#include "enemy.h"
#include <iostream>

Enemy::Enemy(int grid_width, int grid_height) : Snake(grid_width, grid_height) { }

void Enemy::FoodSearch(SDL_Point const food, std::shared_ptr<Barrier> barr){

    SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};

    int x_err = current_cell.x - food.x;
    int y_err = current_cell.y - food.y;

    bool predict_right = Predict(Direction::kRight, barr);
    bool predict_left = Predict(Direction::kLeft, barr);
    bool predict_up = Predict(Direction::kUp, barr);
    bool predict_down = Predict(Direction::kDown, barr);
    bool predict_keep = Predict(direction, barr);

    if (!predict_keep){
        if (direction == Direction::kRight || direction == Direction::kLeft){
            if (y_err > 0){
                if (predict_up)
                    direction = Direction::kUp;
                else
                    direction = Direction::kDown;
            }
            else{
                if (predict_down)
                    direction = Direction::kDown;
                else
                    direction = Direction::kUp;
            }
        }
        else{
            if (x_err > 0){
                if (predict_left)
                    direction = Direction::kLeft;
                else
                    direction = Direction::kRight;
            }
            else
            {
                if (predict_right)
                    direction = Direction::kRight;
                else
                    direction = Direction::kLeft;
            }
        }
    }
    else{
        if ( abs(x_err) > abs(y_err) ) {
            if (x_err > 0 && (direction!= Direction::kRight || size == 1) && predict_left )
                direction = Direction::kLeft;
            else if (x_err < 0 && (direction!= Direction::kLeft|| size == 1) && predict_right )
                direction = Direction::kRight; 
            }
        else{
            if (y_err > 0 && (direction!= Direction::kDown|| size == 1) && predict_up )
                direction = Direction::kUp;
            else if (y_err < 0&& (direction!= Direction::kUp|| size == 1) && predict_down )
                    direction = Direction::kDown;
            }
    }
}

void Enemy::ReStart(){
    static int i = 0;
    if (alive == false ){
        body.clear();
        speed = 0.1;
        i++;
        size = 1;
    }
    if (i%100==99)
        alive = true;
}

bool Enemy::Predict(Direction direc, std::shared_ptr<Barrier> barr){
    float predict_x = head_x;
    float predict_y = head_y;
    bool predict_flag = true;

    if (direc == Direction::kLeft )
        predict_x =head_x - speed;
    else if (direc == Direction::kRight)
        predict_x = head_x + speed;
    else if (direc == Direction::kUp)
        predict_y = head_y - speed;
    else
        predict_y = head_y + speed;

    SDL_Point predict_cell{
    static_cast<int>(predict_x),
    static_cast<int>(predict_y)};

    if ( barr->BarrierCell(predict_cell.x, predict_cell.y) )
        predict_flag = false;

    for (auto point: body){
        if (predict_cell.x == point.x && predict_cell.y==point.y )
            predict_flag = false;
    }

    return predict_flag;
}

void Enemy::psuhBack(Enemy &&v){
    mtx.lock();
    enemies.push_back(std::move(v));
    std::cout << "ID: " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    mtx.unlock();
}

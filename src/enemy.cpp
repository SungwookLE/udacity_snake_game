#include "enemy.h"
#include <iostream>

Enemy::Enemy(int grid_width, int grid_height, int id) : Snake(grid_width, grid_height), _id(id) {
    head_x += (3*id);
    head_y += id;
    std::cout << "Enemy snake #" << _id << " is staged! \n";
}

void Enemy::FoodSearch(SDL_Point const food, std::shared_ptr<Barrier> barr){
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    //std::cout << "Enemy snake #" << this->getID() << " THREAD ID: 0x" <<std::this_thread::get_id() << std::endl;

    SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};
    mtx.lock();
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
            else{
                if (predict_right)
                    direction = Direction::kRight;
                else
                    direction = Direction::kLeft;
                }
        }
    }
    else{
        if (_id % 2 ==0){
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
        else{
            if ( abs(y_err) > 1 && abs(x_err) > 1){
                if ( abs(x_err) < abs(y_err) ) {
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
    }
    mtx.unlock();
}

void Enemy::ReStart(){
    
    if (alive == false ){
        body.clear();
        speed = init_speed;
        count_turn+=1;
        size = 1;
    }
    else{
        count_turn = 0;
    }

    if (count_turn%70==69){
        alive = true;
    }
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

void Enemy::Update(std::shared_ptr<Barrier> barr)   {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    Snake::Update(barr);
    }
 void Enemy::kill_cond(int size_num){
     if (size >= size_num)
         alive = false;
 }
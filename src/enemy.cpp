#include "enemy.h"
#include "iostream"

Enemy::Enemy(int grid_width, int grid_height): Snake(grid_width, grid_height){}
void Enemy::FoodSearch(SDL_Point const food, std::shared_ptr<Barrier> barr){
    
    int x_err = static_cast<int>(head_x) - food.x;
    int y_err = static_cast<int>(head_y) - food.y;
    static Direction i;

    if ( abs(x_err) > abs(y_err) ) {
        if (x_err > 0 && i != Direction::kRight ){
            direction = Direction::kLeft;
        }
        else if (x_err < 0 && i!= Direction::kLeft  )
            direction = Direction::kRight; 
        }
    else{
            if (y_err > 0 && i!= Direction::kDown  )
                direction = Direction::kUp;
            else if (y_err < 0&& i!= Direction::kUp  )
                direction = Direction::kDown;
        }

        i = direction;
}

void Enemy::ReStart(){
    static int i = 0;
    if (alive == false ){
        body.clear();
        speed = 0.1;
        i++;
    }
    if (i%50==49)
        alive = true;
}

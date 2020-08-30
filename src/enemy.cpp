#include "enemy.h"
#include "iostream"

Enemy::Enemy(int grid_width, int grid_height): Snake(grid_width, grid_height){}
void Enemy::FoodSearch(SDL_Point const food){
    
    int x_err = static_cast<int>(head_x) - food.x;
    int y_err = static_cast<int>(head_y) - food.y;
    std::cout << "X_err: " << x_err << "     Y_err: " << y_err << std::endl;
        if (x_err != 0 && y_err != 0){
        
        if ( abs(x_err) > abs(y_err) ) {
            if (x_err > 0)
                direction = Direction::kLeft;
            else if (x_err < 0)
                direction = Direction::kRight; 
        }
        else{
                if (y_err > 0)
                    direction = Direction::kUp;
                else if (y_err < 0)
                    direction = Direction::kDown;
        }

    }

   
}

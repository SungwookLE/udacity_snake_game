#ifndef __ENEMY__H_
#define __ENEMY__H_

#include "snake.h"
#include "SDL.h"
#include <memory>
#include <vector>

class Enemy: public Snake{

public:
    Enemy(int grid_width, int grid_height);
    void FoodSearch(SDL_Point const food, std::shared_ptr<Barrier> barr);
    void ReStart();
    bool Predict(Direction direc, std::shared_ptr<Barrier> barr);

private:
    std::vector<std::vector<unsigned int>> movable_point{4,std::vector<unsigned int>(3,1)};
};





#endif
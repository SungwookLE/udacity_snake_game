#ifndef __ENEMY__H_
#define __ENEMY__H_

#include "snake.h"
#include "SDL.h"
#include <memory>

class Enemy: public Snake{

public:
    Enemy(int grid_width, int grid_height);
    void FoodSearch(SDL_Point const food, std::shared_ptr<Barrier> barr);
    void ReStart();

private:

};





#endif
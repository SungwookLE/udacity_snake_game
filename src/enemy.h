#ifndef __ENEMY__H_
#define __ENEMY__H_

#include "snake.h"
#include "SDL.h"
#include <memory>
#include <vector>
#include <mutex>
#include <future>


class Enemy: public Snake{

    public:
    Enemy(int grid_width, int grid_height);
    void FoodSearch(SDL_Point const food, std::shared_ptr<Barrier> barr);
    void ReStart();
    bool Predict(Direction direc, std::shared_ptr<Barrier> barr);
    int getID() { return _id; }
    

    private:
    int _id{0};
};

#endif
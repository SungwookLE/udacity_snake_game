#ifndef __BARRIER_H_
#define __BARRIER_H_

#include "SDL.h"
#include <iostream>
#include <random>

class Barrier{
    public:
        Barrier(): engine(dev()), random_w(0,30), random_h(0,30){}
        Barrier(std::size_t grid_width, std::size_t grid_height):
         engine(dev()),random_w(0, static_cast<int>(grid_width)),random_h(0, static_cast<int>(grid_height)),
         grid_width(static_cast<int>(grid_width)),grid_height(static_cast<int>(grid_height)) {}
        void Update(int score);
        bool BarrierCell(int x, int y);
        std::vector<SDL_Point> barrier_body;

    private:
        
        std::random_device dev;
        std::mt19937 engine;
        std::uniform_int_distribution<int> random_w;
        std::uniform_int_distribution<int> random_h;
        int grid_width;
        int grid_height;
};

#endif
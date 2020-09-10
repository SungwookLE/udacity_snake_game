#ifndef __ENEMY__H_
#define __ENEMY__H_

#include "snake.h"
#include "SDL.h"
#include <memory>
#include <vector>
#include <future>
#include <thread>
#include <chrono>
#include <mutex>

class Enemy: public Snake{
    public:
    Enemy(int grid_width, int grid_height, int id);
    void FoodSearch(SDL_Point const food, std::shared_ptr<Barrier> barr);
    void ReStart();
    bool Predict(Direction direc, std::shared_ptr<Barrier> barr);
    int getID() { return _id; }
    void Update(std::shared_ptr<Barrier> barr);
    int enemy_score{0};
    void kill_cond(int score_num);

private:
    int _id;
    static std::mutex mtx;
    int count_turn;
};

#endif
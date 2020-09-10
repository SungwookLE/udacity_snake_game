#include "barrier.h"

void Barrier::Update(int score)
{
    int x, y, length;
    x = random_w(engine);
    y = random_h(engine);
    int slope = 0;
    std::uniform_int_distribution<int> dice(10, 15);
    int dice_num = dice(engine);
    //std::cout << "dice num: " << dice_num << std::endl;
    length = score * 2;
    if (length> 80) length = 80;

    barrier_body.resize(length,{0,0});
    barrier_body[0].x = x;
    barrier_body[0].y = y;

    
        for (int i = 1; i < length/2; i++){
            slope++;
            if (slope % dice_num == 0){
                barrier_body.at(i).x = barrier_body.at(i - 1).x + 1;
                barrier_body.at(i).y = barrier_body.at(i - 1).y + 1;
            }
            else{
                barrier_body.at(i).x = barrier_body.at(i - 1).x + 1;
                barrier_body.at(i).y = barrier_body.at(i - 1).y ;
            }
        }
        for (int i = length / 2; i < length; ++i){
            slope++;
            if (slope %dice_num == 0){
                barrier_body.at(i).x = barrier_body.at(i - 1).x - 1 ;
                barrier_body.at(i).y = barrier_body.at(i - 1).y + 1;
            }
            else{
                barrier_body.at(i).x = barrier_body.at(i - 1).x  ;
                barrier_body.at(i).y = barrier_body.at(i - 1).y +1;
            }
        }
    
    

    for (auto &point : barrier_body){
        point.x = fmod(point.x + grid_width, grid_width);
        point.y = fmod(point.y + grid_height, grid_height);
       
        if ( (point.x == grid_width || point.x == 0 )   ){
            point.x -= 1;
        }

        if (  (point.y == grid_height|| point.y == 0 )  ){
            point.y -= 1;
        }
    }
}


bool Barrier::BarrierCell(int x, int y) {
  
  for (auto const &item : barrier_body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}
#include "barrier.h"

void Barrier::Update(int score)
{
    int x, y, length;
    x = random_w(engine);
    y = random_h(engine);
    
    length = score*20;

    barrier_body.resize(length);
    barrier_body[0].x =  x;
    barrier_body[0].y =  y;

    if (score%2){
        for (int i = 1; i < length; i++){
            barrier_body[i].x = barrier_body[i - 1].x + 1;
            barrier_body[i].y = barrier_body[i - 1].y + 1;
        }
    }
    else
    {
        for (int i = 1; i < length; i++){
            barrier_body[i].x = barrier_body[i - 1].x + 1;
            barrier_body[i].y = barrier_body[i - 1].y - 1;
        }
    }
    

    for (auto &point : barrier_body){
        point.x = fmod(point.x + grid_width, grid_width);
        point.y = fmod(point.y + grid_height, grid_height);
       
        if ( (point.x == grid_width )|| (point.x == 0)   ){
            point.x = -1;
        }

        if (  (point.y == grid_height ) || (point.y == 0) ){
            point.y = -1;
        }
    }
}
// 장애물 만나면 스네이크 못 지나가게끔 업데이트 ('20.8/25)

bool Barrier::BarrierCell(int x, int y) {
  
  for (auto const &item : barrier_body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}
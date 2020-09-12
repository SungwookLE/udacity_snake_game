#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"
#include "barrier.h"
#include "enemy.h"

class Renderer
{
public:
  enum class kind_of_food { kill, normal, double_up, life_up };
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();
  void Render(Snake const snake, SDL_Point const &food, std::shared_ptr<Barrier> barrier, std::vector<std::shared_ptr<Enemy>> enemy, int const _num_of_enemy, kind_of_food kind);
  void UpdateWindowTitle(int score, int fps);

  void loadMedia(std::string path){
    //Loading success flag
    bool success = true;

    //Load splash image
    gHelloWorld = SDL_LoadBMP( path.c_str() );
    if( gHelloWorld == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        success = false;
    }
  }

private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  //The surface contained by the window
  SDL_Surface* gScreenSurface = NULL;
  //The image we will load and show on the screen
  SDL_Surface* gHelloWorld = NULL;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
  

};

#endif
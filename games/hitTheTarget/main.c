#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main() {

  SDL_Init(SDL_INIT_VIDEO);
  uint32_t startTime = SDL_GetTicks();

  SDL_Window *window = SDL_CreateWindow( "Mein Spiel",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

  int running = 1;
  SDL_Event event;

  SDL_Rect cannon;
  cannon.x = 50;
  cannon.y = 250;
  cannon.w = 60;
  cannon.h = 20;

  SDL_Rect projectile = {0, 0, 10, 5};
  int projectileActive = 0;

  SDL_Rect target = {700, rand()%550, 50, 50};
  int score = 0;

  srand(time(NULL));

  while(running) {
    while(SDL_PollEvent(&event)) {
      uint32_t currentTime = SDL_GetTicks();
      uint32_t elapsed = currentTime - startTime;
      if(event.type == SDL_QUIT || elapsed > 30000){
        running = 0;
      }

      if(event.type == SDL_KEYDOWN){
        switch(event.key.keysym.sym){
          case SDLK_UP:
            cannon.y -= 10;
            if (cannon.y < 0) cannon.y = 0;
            break;
          case SDLK_DOWN:
            cannon.y += 10;
            if(cannon.y > 580) cannon.y = 580;
            break;
          case SDLK_SPACE:
            if(!projectileActive){
              projectileActive = 1;
              projectile.x = cannon.x + cannon.w;
              projectile.y = cannon.y + cannon.h/2;
            }
            break;
        }
      }
    }
    if(projectileActive){
      projectile.x += 5;
      if(projectile.x > 800)
        projectileActive = 0;
      else if(SDL_HasIntersection(&projectile, &target)){
        score++;
        // print to standard output
        printf("Hit target! Points: %d\n", score);
        projectileActive = 0;
        target.y = rand()%600;
      }
    }

    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    // draw blue cannon
    SDL_SetRenderDrawColor(renderer, 50, 150, 255, 255);
    SDL_RenderFillRect(renderer, &cannon);

    if(projectileActive){
      // draw red projectile
      SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
      SDL_RenderFillRect(renderer, &projectile);
    }

    // draw green target
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &target);

    // Draw orange points
    for(int i = 0; i < score; i++){
      SDL_Rect point = {10 + i*15, 10, 10, 10};
      SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255); // orange
      SDL_RenderFillRect(renderer, &point);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(5);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  printf("Game finished\n");
  printf("Points: %d\n", score);
  usleep(3);

  return 0;
}

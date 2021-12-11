
#ifndef INCLUDES_H_INCLUDED
#include "includes.h"
#endif

#ifndef CONSTANTS_H_INCLUDED
#include "constants.h"
#endif

#ifndef UNIVERSE_H_INCLUDED
#include "universe.h"
#endif

int main(int argc, char* args[] )
{
    SDL_Window *window;
	SDL_Renderer *renderer;

	TTF_Init();
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Arys",0,0,SCREEN_X,SCREEN_Y,0);
	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
	//SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);

    int myseed = 1637084828;
    int seeded = 0;


    int seed = -13;
	if(seeded==0)
    {
        seed = time(NULL);
    }
    else
    {
        seed = myseed;
    }
    srand(seed);
    printf("Random seed:%d\n",seed);
	int delay = 0;
	int fullscreen = 0;


    Universe universe;
	Universe_Initiate(&universe);

	float scale = ((float)SCREEN_Y)/((float)UNIVERSE_SIZE*1.1f);
	float beginscale = scale;
	float beginx = 150.0f;
	float beginy = 150.0f;
	float x = beginx;
	float y = beginy;
	float dt = 1000.0f;

	int running = 1;

    char c[128];
    int frames = 0;
    int mx,my;
    int doe = 0;
    int info = 0;
    int info2 = 0;

    clock_t start, stop;

    SDL_Event event;

    clock_t beginn = clock();

	while(running==1)
	{
		start = clock();

        mx = NULL;
        my = NULL;

		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_MOUSEBUTTONDOWN:
					switch(event.button.button)
					{
                        case SDL_BUTTON_LEFT:
                            if(mx==NULL||mx==-13)
                            {
                                SDL_GetMouseState(&mx,&my);
                            }
                            break;
                        case SDL_BUTTON_RIGHT:
                            mx = -13;
                            break;
					}
					break;
				case SDL_MOUSEWHEEL:

					scale += event.wheel.y*0.1;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							running = 0;
							break;
						case SDLK_RIGHT:
							x -= 20.0f;
							break;
						case SDLK_d:
							x -= 20.0f;
							break;
						case SDLK_LEFT:
							x += 20.0f;
							break;
						case SDLK_q:
							x += 20.0f;
							break;
						case SDLK_UP:
							y += 20.0f;
							break;
						case SDLK_z:
							y += 20.0f;
							break;
						case SDLK_DOWN:
							y -= 20.0f;
							break;
						case SDLK_s:
							y -= 20.0f;
							break;
						case SDLK_r:
							x = beginx;
							y = beginy;
							scale = beginscale;
							break;
                        case SDLK_g:
							universe.gravity = (universe.gravity+1)%2;
							break;
						case SDLK_i:
                            info = (info + 1)%2;
							break;
						case SDLK_j:
                            info2 = (info2 + 1)%2;
							break;
                        case SDLK_k:
                            doe = (doe + 1)%2;
							break;
						case SDLK_f:
							break;
						case SDLK_x:
							break;
						case SDLK_y:
							break;

					}

			}

		}
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderClear(renderer);

        if(doe==1)
        {
            Universe_Run(&universe);
        }
		Universe_Blits(renderer,&universe,scale,x,y,mx,my);
        if(info==1)
        {

            snprintf(c,sizeof c,"fps %d",(int)dt);
            draw_text(renderer,20,20,250,250,250,c);
            if(info2==1)
            {

                snprintf(c,sizeof c,"amount %d",(int)universe.amount);
                draw_text(renderer,20,40,250,250,250,c);

                float elapsed = (float)(clock()-beginn)/60000.0f;
                snprintf(c,sizeof c,"elapsed %f",elapsed);
                draw_text(renderer,20,60,250,250,250,c);

                //snprintf(c,sizeof c,"gravity %d",universe.gravity);
                //draw_text(renderer,20,80,250,250,250,c);

            }
        }


		SDL_RenderPresent(renderer);
		stop = clock();
		dt = 1000.0f/(float)(stop - start);
		//printf("fps: %d\n",(int)dt);
	}


    printf("\nRandom seed: %d\n",seed);

	SDL_DestroyWindow(window);

	SDL_DestroyRenderer(renderer);

	SDL_Quit();

	return 0;
}


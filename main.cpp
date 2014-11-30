// ------------------------------------------------------------------------------
// Mandelbrot - SDL
// Date: 30.11.2014
// Version: none
// Author: Maciej Dziemianczuk
// ------------------------------------------------------------------------------

//#define __DEBUG
#include "main.h"

#include "Threads.h"
#include "Mandelbrot.h"
#include "SDL_interface.h"

#undef main


// What would you like to see? Mandelbrot or Julia?
enum MODE mode = MANDELBROT;
// mode = JULIA;


// ------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    // Screen resolution
    World.Width = 800;
    World.Height = 800;
    // Number of threads
    NUM_THREADS = 1;


    // Initialize SDL video
    if (SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    }
    // Register SDL_Quit to be called at exit;
    atexit(SDL_Quit);

    // Attempt to create a WIDTH x HEIGHT window with 32bit pixels depth
    Screen = SDL_SetVideoMode(World.Width, World.Height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    // init parallel stuff: SDL_threads, mutex, etc
    InitParallel();

	// calculate image
	DrawingParallel();

	// Mouse
	int Mouse_x = 0, Mouse_y = 0;
	bool Mouse_first = true;

    while (true)
    {
        // Lock surface if needed
        if (SDL_LockSurface(Screen) < 0) return 1;

        // Unlock if needed
        if (SDL_MUSTLOCK(Screen))
           SDL_UnlockSurface(Screen);


        // Update the whole screen
        SDL_UpdateRect(Screen, 0, 0, World.Width, World.Height);

        // Poll for events, and handle the ones we care about
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
				// ----------------------------
                case SDL_KEYDOWN:
                    break;

				// ----------------------------
                case SDL_KEYUP:
                    switch (event.key.keysym.sym)
                    {
						case SDLK_F1:
							printf("HELP:\n\n1) ENLARGE - click the left-top corner and then the right-bottom one.\n2) EXIT - press Esc\n  \n");
                    		break;

                    	case SDLK_ESCAPE:
                    		CleanParallel();
                        	return 0;

						case SDLK_LEFT:
							parametrA += 0.01;
							break;

						case SDLK_RIGHT:
							parametrA -= 0.01;
                        	break;

						case SDLK_UP:
                        	parametrB += 0.01;
                        	break;

						case SDLK_DOWN:
							parametrB -= 0.01;
							break;
                    }

					DrawingParallel();
					break;

				// ----------------------------
                case SDL_MOUSEBUTTONDOWN:
                {

                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        if (Mouse_first)
                        {
                            DOUBLE dx = (end_x - start_x)/(DOUBLE)World.Width;
                            DOUBLE dy = (end_y - start_y)/(DOUBLE)World.Height;

							__PRINTF("The First Click\n");
                            Mouse_x = event.button.x;
                            Mouse_y = event.button.y;
                            Mouse_first = false;

							drawRect(event.button.x, event.button.y, 2, 0xffffff);

                            __PRINTF("Range x %lf %lf \n", start_x, end_x);
                            __PRINTF("Range y %lf %lf \n", start_y, end_y);
                            __PRINTF("Pos XY: %lf %lf \n", start_x + dx * Mouse_x, start_y + dy*Mouse_y);
                        }
                        else
                        {
                            __PRINTF("The Second click\n");

							drawRect(event.button.x, event.button.y, 2, 0xffffff);

                            DOUBLE dx = (end_x - start_x)/(DOUBLE)World.Width;
                            DOUBLE dy = (end_y - start_y)/(DOUBLE)World.Height;

                            __PRINTF("Pos XY: %lf %lf \n", start_x + dx * Mouse_x, start_y + dy*Mouse_y);

							int rect;
							if (abs(Mouse_x - event.button.x) > abs(Mouse_y - event.button.y))
								rect = abs(Mouse_y - event.button.y);
							else
								rect = abs(Mouse_x - event.button.x);


                            //end_x = start_x + dx * event.button.x;
                            //end_y = start_y + dy * event.button.y;
							end_x = start_x + dx * (Mouse_x+rect);
                            end_y = start_y + dy * (Mouse_y+rect);

 							start_x = start_x + dx * Mouse_x;
                            start_y = start_y + dy * Mouse_y;

                            __PRINTF("Range x %lf %lf \n", start_x, end_x);
                            __PRINTF("Range y %lf %lf \n", start_y, end_y);

                            Mouse_first = true;

							DrawingParallel();
                        }
                    }
                }
				break;

				// ----------------------------
                case SDL_QUIT:
                {

                    CleanParallel();
                    return 0;
                }


            }
        }

    }


    CleanParallel();
    return 0;
}


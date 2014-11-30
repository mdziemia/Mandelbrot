#include "main.h"
#include "Threads.h"
#include "Mandelbrot.h"
#include "SDL_interface.h"

#define TAB(x,y,mem) *(mem+World.Width*(y)+(x))

const int COLOR = 0xff0000;     // color of cells
const int COLOR_NEW = 0x00ff00; // color of born cells
const int COLOR_BG = 0x000000;
const int COLOR_DEAD = 0x000000;

SWorld World;

DOUBLE start_x = -3.0, end_x = 1.0;
DOUBLE start_y = -1.5, end_y = 1.5;

DOUBLE parametrA = 0.3;
DOUBLE parametrB = - 0.13;



// ----------------------------------------------------------------------------------
int Julia(DOUBLE _x, DOUBLE _y)
{
	DOUBLE x = _x;
    DOUBLE y = _y;

   	DOUBLE nx, ny;

    for (int i=0; i<300; ++i)
    {
        nx = x*x - y*y + parametrA;
        ny = 2*x*y     - parametrB;

        x = nx;
        y = ny;

        if (sqrtl(x*x + y*y) > 2) return i;
    }
    return true;
}

// ----------------------------------------------------------------------------------
int Mandelbrot(DOUBLE _x, DOUBLE _y)
{
    DOUBLE x = _x;
    DOUBLE y = _y;

    DOUBLE nx, ny;

    for (int i=0; i<256; ++i)
    {
        nx = x*x - y*y + _x;
        ny = 2*x*y + _y;

        x = nx;
        y = ny;

        if (sqrtl(x*x + y*y) > 2.0) return i;
    }
    return 0;
}

// ----------------------------------------------------------------------------------
// Which part of screen will be computed
void Drawing(int height_start, int height_end, int thread_id)
{

    DOUBLE dx = (end_x - start_x)/(DOUBLE)World.Width;
    DOUBLE dy = (end_y - start_y)/(DOUBLE)World.Height;
    int iter;

    //if (thread_id != 0) return 0;
    int x, y;

    __PRINTF("Drawing start %f %f \n", start_x, start_y);
    __PRINTF("Drawing end %f %f \n", end_x, end_y);

    for (y = height_start; y< height_end; ++y)
    {
        for (x = 0; x < World.Width; ++x)
        {
        	switch (mode)
        	{
        		case JULIA:
        			if ((iter = Julia(start_x + x*dx,start_y + y*dy)) > 1)
        				putpixel(x, y, COLOR_NEW*iter);
           			else
           				putpixel(x, y, COLOR_BG);
           			break;

				case MANDELBROT:
					if ((iter = Mandelbrot(start_x + x*dx,start_y + y*dy)) > 1)
						putpixel(x, y, COLOR_NEW*iter);
           			else
           				putpixel(x, y, COLOR_BG);
					break;

        	}
        }
    }
}



// ----------------------------------------------------------------------------------
// This is the main method which runs at every THREAD
int ComputeParallel(void *_ptrThread)
{
    if (!_ptrThread) return 0;
    SThread *ptrThread = (SThread *)_ptrThread;


    // calculate range of rows to deal with
    int range_of_height = (int)(World.Height / NUM_THREADS);
    int height_start = ptrThread->id*range_of_height;
    int height_end = (ptrThread->id+1)*range_of_height;
    if (ptrThread->id == NUM_THREADS - 1)
        height_end = World.Height;


    // main loop
    while (ptrThread->status != THREAD_STATUS_QUIT)
    {

        SDL_LockMutex(ptrThread->hMutex);
        if (ptrThread->status == THREAD_STATUS_WAIT)
        {
            SDL_UnlockMutex(ptrThread->hMutex);
            continue;
        }
        else if (ptrThread->status != THREAD_STATUS_QUIT)
        {
            ptrThread->status = THREAD_STATUS_WAIT;
            SDL_UnlockMutex(ptrThread->hMutex);
        }


        // Calculation at specified range
        Drawing(height_start, height_end, ptrThread->id);


        // Critical Section
        SDL_LockMutex(lockSynchMutex);
            cSynchronize ++;
        SDL_UnlockMutex(lockSynchMutex);

    }
    return 0;
}

// ----------------------------------------------------------------------------------
void DrawingParallel(void)
{
    // Critical section
    SDL_LockMutex(lockSynchMutex);
    if (cSynchronize != NUM_THREADS)
    {
        SDL_UnlockMutex(lockSynchMutex);
        return;
    }
    cSynchronize = 0;
    SDL_UnlockMutex(lockSynchMutex);

    for (int i=0; i<NUM_THREADS; ++i)
    {
        // Critical section
        SDL_LockMutex(arrThreads[i].hMutex);
            arrThreads[i].status = THREAD_STATUS_DEFAULT;
        SDL_UnlockMutex(arrThreads[i].hMutex);
    }
}













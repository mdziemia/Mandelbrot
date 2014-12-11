#ifndef __MAIN
#define __MAIN

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL.h>
#include <SDL_thread.h>

#ifdef __DEBUG
	#define __PRINTF	printf
#else
	#define __PRINTF	if (false) printf
#endif


enum MODE { MANDELBROT, JULIA, BURNING_SHIP };
extern enum MODE mode;

// resolution of computation
typedef long double DOUBLE;


// Parameters for Julia
extern DOUBLE parametrA;
extern DOUBLE parametrB;


#endif

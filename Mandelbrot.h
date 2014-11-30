#ifndef __GAME_OF_LIFE
#define __GAME_OF_LIFE


typedef struct SWorld
{
    unsigned short Width;
    unsigned short Height;

    // Array for calculation
    char *ArrayFirst;
    char *ArraySecond;

    // Array for colors of cells
    short *Colors;


} SWorld;

extern SWorld World;

int ComputeParallel(void *_ptrThread);
void DrawingParallel(void);

extern DOUBLE start_x, end_x;
extern DOUBLE start_y, end_y;


#endif


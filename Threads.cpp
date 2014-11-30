#include "main.h"

#include "Threads.h"
#include "Mandelbrot.h"

SThread *arrThreads;
unsigned short NUM_THREADS;

//SDL_mutex *hMutex;
SDL_mutex *hMutexScreen;
SDL_mutex *lockSynchMutex;

unsigned short cSynchronize;

// ----------------------------------------------------------------------------------
void InitParallel(void)
{
    printf("Init start\n");

    // Create threads
    //NUM_THREADS = 1;//omp_get_num_procs();


    arrThreads = (SThread *)malloc(sizeof(SThread)*NUM_THREADS);


    for (int i=0;i<NUM_THREADS;++i)
    {
        arrThreads[i].id = i;
        arrThreads[i].status = THREAD_STATUS_WAIT;
        arrThreads[i].hMutex = SDL_CreateMutex();
        arrThreads[i].cond = SDL_CreateCond();


        arrThreads[i].handle = SDL_CreateThread(ComputeParallel, (void *)&arrThreads[i]);
        if (!arrThreads[i].handle)
        {
            printf("Cannot create thread %d!\n", i);
            exit(1);
        }

    }

    // Create Mutex to synchronize calculation
    //hMutex = SDL_CreateMutex();
    //hMutexScreen = SDL_CreateMutex();
    lockSynchMutex = SDL_CreateMutex();

    SDL_mutexP(lockSynchMutex);
        //SDL_mutexP(hMutex);
            cSynchronize = NUM_THREADS;;
        //SDL_mutexV(hMutex);
    SDL_mutexV(lockSynchMutex);




    printf("Init ok\n");
}

// ----------------------------------------------------------------------------------
void CleanParallel(void)
{
    printf("Ending ...");

    // Tell to threads to quit
    for (int i=0; i<NUM_THREADS; ++i)
    {
        SDL_LockMutex(arrThreads[i].hMutex);
        arrThreads[i].status = THREAD_STATUS_QUIT;
        SDL_UnlockMutex(arrThreads[i].hMutex);
    }

    // Wait for thread to finish
    for (int i=0; i<NUM_THREADS; ++i)
    {
        SDL_WaitThread(arrThreads[i].handle, NULL);
    }

    // Destroy mutex
    if (lockSynchMutex) SDL_DestroyMutex(lockSynchMutex);
    // if (hMutexScreen) SDL_DestroyMutex(hMutexScreen);

    if (arrThreads) free(arrThreads);
    printf("... done.\n");

}


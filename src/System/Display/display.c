#include <stdio.h>
#include <SDL2/SDL.h>

#include "display.h"

#include "log.h"

//The window we'll be rendering to
SDL_Window* disp_Window = NULL;

//The surface contained by the window
SDL_Surface* disp_ScreenSurface = NULL;

void init_display(char title[], int width, int height)
{
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        log_fatal( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }

    //Create window
    disp_Window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width,
            height,
            SDL_WINDOW_SHOWN
    );

    if(disp_Window == NULL )
    {
        log_fatal( "disp_Window could not be created! SDL_Error: %s\n", SDL_GetError() );
    }

    //Get window surface
    disp_ScreenSurface = SDL_GetWindowSurface(disp_Window );
}

void close_display()
{
    //Destroy window
    SDL_DestroyWindow(disp_Window );
    disp_Window = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

void blit_bitmap_32bppRGBA(uint32_t raw[], int width, int height) {
    SDL_Surface* gBitmap = SDL_CreateRGBSurfaceFrom(
            raw,
            width,
            height,
            32,
            width * 4,
            0x00ff0000,
            0x0000ff00,
            0x000000ff,
            0
    );

    //Load media
    if( gBitmap == NULL )
    {
        log_warn( "Failed to load media: %s\n", SDL_GetError() );
        return;
    }

    //Apply the image
    SDL_BlitScaled(gBitmap, NULL, disp_ScreenSurface, NULL);

    //Update the surface
    SDL_UpdateWindowSurface(disp_Window );

    SDL_FreeSurface(gBitmap);
}


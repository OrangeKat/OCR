#include <err.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>


// Updates the display.
//
// renderer: Renderer to draw on.
// texture: Texture that contains the image.
void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

}

// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
// colored: Texture that contains the colored image.
// grayscale: Texture that contains the grayscale image.
void event_loop(SDL_Renderer* renderer, SDL_Texture* grayscale)
{
    SDL_Event event;
    SDL_Texture* t = grayscale;


    while (1)
    {
        SDL_WaitEvent(&event);

        draw(renderer, t);

        switch (event.type)
        {
            // If the "quit" button is pushed, ends the event loop.
            case SDL_QUIT:
                return;
            //CHANGE FROM GRAYSCALE TO COLORED WHEN PRESSED
            case SDL_KEYDOWN:
                break;
                // If the window is resized, updates and redraws the diagonals.
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    draw(renderer, t);
                }
                break;

        }
    }
}

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* load_image(const char* path)
{
    SDL_Surface* surface = IMG_Load(path);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface* newSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);
    if (newSurface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_FreeSurface(surface);
    return newSurface;
}

// Converts a colored pixel into grayscale.
//
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    Uint8 average = 0.3*r + 0.59*g + 0.11*b;

    r = average;
    g = average;
    b = average;

//    Uint8 Y = 0.2126*r + 0.7152*g + 0.0722*b;


//    if (Y < 130)
  //  {
//	    r = 0;
//	    g = 0;
//	    b = 0;
  //  }
   // else 
   // {
//	    r = 255;
//	    g = 255;
//	    b = 255;
  //  }
    Uint32 color = SDL_MapRGB(format, r, g, b);
    return color;
}


Uint32 pixel_to_blackwhite(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    Uint8 Y = 0.2126*r + 0.7152*g + 0.0722*b;


    if (Y < 130)
    {
	    r = 0;
	    g = 0;
	    b = 0;
    }
    else 
    {
	    r = 255;
	    g = 255;
	    b = 255;
    }
    Uint32 color = SDL_MapRGB(format, r, g, b);
    return color;
}

void surface_to_grayscale(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;
    SDL_LockSurface(surface);
    int count = 0 ;
    while (count < len)
    {
        Uint32 color = pixel_to_grayscale(pixels[count], format);
        pixels[count] = color;
        count++;
    }
   count = 0; 
    while (count < len)
    {
//        Uint32 color = pixel_to_blackwhite(pixels[count], format);
//        pixels[count] = color;
        count++;
    }




}

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    //WINDOW
    SDL_Window* window = SDL_CreateWindow("Display image", 0, 0, 20, 20,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    // IF WINDOW NOT WORKING THEN ERROR
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    //RENDERER
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //IF RENDERER NOT WORKING THEN ERROR
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    //SURFACE

    SDL_Surface* surfaceColored = load_image(argv[1]);
    if (surfaceColored == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_SetWindowSize(window, surfaceColored->w, surfaceColored->h);


    //TEXTURE COLORED
    SDL_Texture* textureColored = SDL_CreateTextureFromSurface(renderer, surfaceColored);

    if (textureColored == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    surface_to_grayscale(surfaceColored);



    //TEXTURE GRAYSCALE
    SDL_Texture *textureGrayscale = SDL_CreateTextureFromSurface(renderer, surfaceColored);

    if (textureGrayscale == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    IMG_SavePNG(surfaceColored, "grayscale.png");


    SDL_FreeSurface(surfaceColored);


    event_loop(renderer, textureGrayscale);
    SDL_DestroyTexture(textureColored);
    SDL_DestroyTexture(textureGrayscale);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;




    // TODO:
    // - Initialize the SDL.
    // - Create a window.
    // - Create a renderer.
    // - Create a surface from the colored image.
    // - Resize the window according to the size of the image.
    // - Create a texture from the colored surface.
    // - Convert the surface into grayscale.
    // - Create a new texture from the grayscale surface.
    // - Free the surface.
    // - Dispatch the events.
    // - Destroy the objects.

}

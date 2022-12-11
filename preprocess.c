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
}

int coord(int i, int j, int w)
{
    return w*i + j;
}

Uint8 get_med(Uint8 matrix[])
{
    Uint8 temp = 0;
    for (size_t i = 0; i < 8; i++)
    { 
        Uint8 min = matrix[i];
        int index = i;
        for (size_t j = i+1; j < 9; j++)
        {
            if (matrix[j] < min)
            {
                min = matrix[j];
                index = j;
            }
        }
        temp = matrix[i];
        matrix[i] = matrix[index];
        matrix[index] = temp;
    }
    return matrix[4];
}

Uint8 get_moy(Uint8 matrix[])
{
    int sum = 0;
    for (size_t i = 0; i < 9; i++)
    {
        sum += matrix[i];
    }
    return sum/9;
    
}

Uint8 med_neighbour(int a, int b, SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int w = surface->w;
    int p = coord((a-1),(b-1),w); //position of our base point
    SDL_PixelFormat* format = surface->format;
    Uint8 r, g, bl;
    
    Uint8 matrix[9];

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {   
            SDL_GetRGB(pixels[p], format, &r, &g, &bl);
            matrix[i*3 + j] = r;
            p++;
        }       
        p += w;
    }
    
    return get_med(matrix);
}

SDL_Surface* noise_canceler(SDL_Surface* surface)
{
    int w = surface->w;
    int h = surface->h;
    Uint8 r, g, b;
    Uint32 color;
    SDL_Surface* result = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGB888);
    Uint32* pixels = result->pixels;
    SDL_PixelFormat* format = result->format;

    for (int i = 3; i < h-3; i++)
    {
        for (int j = 3; j < w-3; j++)
        {
            r = med_neighbour(i,j,surface);
            g = r;
            b = r;
            color = SDL_MapRGB(format, r, g, b);
            pixels[coord(i,j,w)] = color;
        }
    }
    return result;
}

unsigned int get_histo(SDL_Surface* surface, unsigned int histo[])
{
    unsigned int sum = 0;
    //resets the value of the histo in the memory
    for (size_t i = 0; i < 255; i++)
        histo[i] = 0;

    //getting all the data we need
    int w = surface->w;
    int h = surface->h;
    Uint32* pixels = surface->pixels;
    SDL_PixelFormat* format = surface->format;
    Uint8 r, g, bl;

    //incrementing to get the whole histogram
    for (int i = 0; i < w*h; i++)
    {
        
        SDL_GetRGB(pixels[i], format, &r, &g, &bl);
        histo[r] += 1;
        sum += 1;
    }    
    return sum;
}

void get_coef(unsigned int *histo, float*coef, unsigned int sum)
{
    //reset the value of the coef array in teh memory, 
    for (size_t i = 0; i < 255; i++)
        coef[i] = 0;

    //initialize the local sums
    long back_sum = histo[0] +1;
    long fore_sum = sum - histo[0];
    long back_weighted = 1;
    long fore_weighted = 0;
    for (size_t i = 1; i < 255; i++)
    {
        fore_weighted += histo[i]*i;
    }

    //initialize the weights 
    float Wb = 0;
    float Wf = 0;
    float Mb = 0;
    float Mf = 0;

    for (size_t i = 1; i < 255; i++)
    {
        //computes the weights at rank i
        Wb = (float)back_sum/sum;
        Wf = (float)fore_sum/sum;
        Mb = (float)back_weighted/back_sum;
        Mf = (float)fore_weighted/fore_sum;
        //store the otsu value with threshold i inside coef
        coef[i] = Wb * Wf * ((Mb-Mf)*(Mb-Mf));
        //updates the local sums
        back_sum += histo[i];
        fore_sum -= histo[i];
        back_weighted += histo[i]*i;
        fore_weighted -= histo[i]*i;
    }
}

int get_max(float * coef)
{
    int max = 0;
    int max_val = coef[0];
    for (size_t i = 1; i < 255; i++)
    {
        if(coef[i] > max_val)
        {
            max_val = coef[i];
            max = i;
        }
    }
    return max;
}

void binarize(SDL_Surface* surface, int threshold)
{
    //getting all the data we need1
    int w = surface->w;
    int h = surface->h;
    Uint32* pixels = surface->pixels;
    SDL_PixelFormat* format = surface->format;
    Uint8 r, g, bl;
    Uint32 white = SDL_MapRGB(format, 0, 0, 0);
    Uint32 black = SDL_MapRGB(format, 255, 255, 255);


    //incrementing to get the whole histogram
    for (int i = 0; i < w*h; i++)
    {
        SDL_GetRGB(pixels[i], format, &r, &g, &bl);
        if (r > threshold)
            pixels[i] = white;
        else
            pixels[i] = black; 
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
    SDL_Surface* surface = load_image(argv[1]);
    if (surface== NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_SetWindowSize(window, surface->w, surface->h);


    surface_to_grayscale(surface);

    //surface = noise_canceler(surface);
 
    unsigned int histo[256];
    unsigned int sum = get_histo(surface,histo);
    float coef[256];
    get_coef(histo,coef,sum);
    int threshold = get_max(coef);
    binarize(surface,threshold);

    //TEXTURE 
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (texture == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());



    IMG_SavePNG(surface, "preprocess.png");


    SDL_FreeSurface(surface);


    event_loop(renderer, texture);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;

}

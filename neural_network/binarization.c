#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
#include "neural_network.h"

/*
    TODO:
    - Function that resizes and image to 100x100 (Make it so you can chos the size)
    - Function that converts a black and white image to a 2D array of 0s and 1s
*/

// Function that resizes an image to an nxn image
SDL_Surface *resize_image(SDL_Surface *image, int n){
    SDL_Surface *resized_image = SDL_CreateRGBSurface(0,n,n,32,0,0,0,0);
    SDL_BlitScaled(image,NULL,resized_image,NULL);
    return resized_image;
}

// Function that converts a array of 0s and 1s to a black and white image
Uint32 getpixel(SDL_Surface *surface, int x, int y){
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    switch(bpp){
        case 1:
            return *p;
        case 2:
            return *(Uint16 *)p;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4:
            return *(Uint32 *)p;
        default:
            return 0;
    }
}

// Function that converts a black and white image to a array of 0s and 1s
double *convert_to_array(char path[]){
    SDL_Surface *image = IMG_Load(path);
    resize_image(image, res);
    int height = image->h;
    int width = image->w;
    double *array = malloc(height * width * sizeof(int *));
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            Uint32 pixel = getpixel(image,i,j);
            Uint8 r,g,b;
            SDL_GetRGB(pixel,image->format,&r,&g,&b);
            if (r == 0 && g == 0 && b == 0){
                array[i * height + j] = 1.0f;
            } else {
                array[i * height + j] = 0.0f;
            }
        }
    }
    return array;
}


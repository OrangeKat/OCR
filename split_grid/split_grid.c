#include "split_grid.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Function that removes the border lines from a cell
#include <SDL2/SDL.h>

#include <SDL2/SDL.h>

void remove_border_lines(SDL_Surface *surface) {
	int top = -1;
    int bottom = -1;
    int n = surface->h;
    Uint32* pixels = surface->pixels;
    for (int i = 0; i < n; i++){
        Uint8 r, g, b;
        SDL_GetRGB(pixels[i * n + i], surface->format, &r, &g, &b);
        if ((r + g + b) / 3 > 128){
            top = i;
            break;
        }
    }
    for (int i = n - 1; i >= 0; i--){
        Uint8 r, g, b;
        SDL_GetRGB(pixels[i * n + i], surface->format, &r, &g, &b);
        if ((r + g + b) / 3 > 128){
            bottom = i;
            break;
        }
    }
    SDL_Rect rect;
    rect.x = top;
    rect.y = top;
    rect.w = bottom - top;
    rect.h = bottom - top;
    SDL_Surface *new_surface = SDL_CreateRGBSurface(0,rect.w,rect.h,32,0,0,0,0);
    SDL_BlitSurface(surface,&rect,new_surface,NULL);
    SDL_BlitSurface(new_surface,NULL,surface,NULL);
}


//function that splits a png of a sudoku grid into 81 images of the individual cells
void split_image(char *filename){
    SDL_Surface *image = IMG_Load(filename);
    int height = image->h;
    int width = image->w;
    int cell_height = height/9;
    int cell_width = width/9;
    int x = 0,y = 0;
    int n = 1;
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            SDL_Rect rect = {x,y,cell_width,cell_height};
            SDL_Surface *cell = SDL_CreateRGBSurface(0,cell_width,cell_height,32,0,0,0,0);
            SDL_BlitSurface(image,&rect,cell,NULL);
            //remove_border_lines(cell);
            char *cell_name = malloc(28);
	        sprintf(cell_name, "output/cell_%d.png",n);
            IMG_SavePNG(cell,cell_name);
            x += cell_width;
            n++;
        }
        x = 0;
        y += cell_height;
    }
}

//splits an image into 9 images of a sudoku grids cells
void split_cells(SDL_Surface *surface, int *n){
    int height = surface -> h;
    int width = surface -> w;
    int cell_height = height/9;
    int cell_width = width/9;
    int x = 0,y = 0;
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            SDL_Rect rect = {x,y,cell_width,cell_height};
            SDL_Surface *cell = SDL_CreateRGBSurface(0,cell_width,cell_height,32,0,0,0,0);
            SDL_BlitSurface(surface,&rect,cell,NULL);
            //remove_border_lines(cell);
            char *cell_name = malloc(28);
            sprintf(cell_name, "output/cell_%d.png", *n);
            IMG_SavePNG(cell,cell_name);
            x += cell_width;
            *n += 1;
        }
        x = 0;
        y += cell_height;
    }
}

//splits an image into 9 images of a sudoku grids squares
void split_squares(SDL_Surface *surface){
    int height = surface -> h;
    int width = surface -> w;
    int square_height = height/3;
    int square_width = width/3;
    int x = 0,y = 0;
    int *n = malloc(sizeof(int));
    *n = 1;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            SDL_Rect rect = {x,y,square_width,square_height};
            SDL_Surface *square = SDL_CreateRGBSurface(0,square_width,square_height,32,0,0,0,0);
            SDL_BlitSurface(surface,&rect,square,NULL);
            //remove_border_lines(square);
            split_cells(square, n);
            x += square_width;
        }
        x = 0;
        y += square_height;
    }
}

// Function that resizes an image to an nxn image
SDL_Surface *resize_image(SDL_Surface *image, int n){
    SDL_Surface *resized_image = SDL_CreateRGBSurface(0,n,n,32,0,0,0,0);
    SDL_BlitScaled(image,NULL,resized_image,NULL);
    return resized_image;
}

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Please provide a filename\n");
        return 1;
    }
    SDL_Surface *image = IMG_Load(argv[1]);
    image = resize_image(image, 900);
    split_squares(image);
    return 0;
}

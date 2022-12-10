#include "split_grid.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Function that removes the border lines from a cell
void remove_border_lines(SDL_Surface *surface) {
    SDL_LockSurface(surface);

    // Loop through the pixels at the top and bottom of the image and find the first and last non-black pixels
    int top = -1;
    int bottom = -1;
    for (int i = 0; i < surface->w; i++) {
        for (int j = 0; j < surface->h; j++) {
            Uint32 *pixel = (Uint32*)surface->pixels + j * surface->w + i;
            Uint8 r, g, b;
            SDL_GetRGB(*pixel, surface->format, &r, &g, &b);
                if ((r + g + b) / 3 > 128) {
                    if (top == -1) {
                        top = j;
                }
                bottom = j;
            }
        }
    }

    // Loop through the pixels at the left and right of the image and find the first and last non-black pixels
    int left = -1;
    int right = -1;
    for (int i = 0; i < surface->h; i++) {
        for (int j = 0; j < surface->w; j++) {
            Uint32 *pixel = (Uint32*)surface->pixels + i * surface->w + j;
            Uint8 r, g, b;
            SDL_GetRGB(*pixel, surface->format, &r, &g, &b);
            if ((r + g + b) / 3 > 128) {
                if (left == -1) {
                    left = j;
                }
                right = j;
            }
        }
    }

    // Calculate the new width and height of the image
    int new_width = right - left + 1;
    int new_height = bottom - top + 1;

    // Create a new SDL_Surface with the new width and height
    SDL_Surface *new_surface = SDL_CreateRGBSurface(0, new_width, new_height, 32, 0, 0, 0, 0);

    // Loop through the pixels in the new surface and copy the corresponding pixels from the original surface
    for (int i = 0; i < new_width; i++) {
        for (int j = 0; j < new_height; j++) {
            Uint32 *old_pixel = (Uint32*)surface->pixels + (j + top) * surface->w + (i + left);
            Uint32 *new_pixel = (Uint32*)new_surface->pixels + j * new_surface->w + i;
            *new_pixel = *old_pixel;
        }
    }

    SDL_UnlockSurface(surface);

    // Replace the original surface with the new one
    *surface = *new_surface;
}

//function that splits a png of a sudoku grid into 81 images of the individual cells
void split_image(char *filename){
    SDL_Surface *image = IMG_Load(filename);
    int height = image->h;
    int width = image->w;
    int square_height = height/3;
    int square_width = width/3;
    int cell_height = height/9;
    int cell_width = width/9;
    int x = 0,y = 0;
    int n = 1;
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            SDL_Rect rect = {x,y,cell_width,cell_height};
            SDL_Surface *cell = SDL_CreateRGBSurface(0,cell_width,cell_height,32,0,0,0,0);
            SDL_BlitSurface(image,&rect,cell,NULL);
            remove_border_lines(cell);
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
void split_cells(SDL_Surface *surface, int square_number){
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
            remove_border_lines(cell);
            char *cell_name = malloc(28);
            sprintf(cell_name, "output/cell_%d.png", square_number + 10 * (i+j));
            IMG_SavePNG(cell,cell_name);
            x += cell_width;
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
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            SDL_Rect rect = {x,y,square_width,square_height};
            SDL_Surface *square = SDL_CreateRGBSurface(0,square_width,square_height,32,0,0,0,0);
            SDL_BlitSurface(surface,&rect,square,NULL);
            remove_border_lines(square);
            split_cells(square, i + j);
            x += square_width;
        }
        x = 0;
        y += square_height;
    }
}

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Please provide a filename\n");
        return 1;
    }
    SDL_Surface *image = IMG_Load(argv[1]);
    split_squares(image);
    return 0;
}

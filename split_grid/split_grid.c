#include "split_grid.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// function that resizes an image to an nxn image
SDL_Surface *resize_image(SDL_Surface *image, int n){
    SDL_Surface *resized_image = SDL_CreateRGBSurface(0,n,n,32,0,0,0,0);
    SDL_BlitScaled(image,NULL,resized_image,NULL);
    return resized_image;
}

//function that splits a png of a sudoku grid into 81 images of the individual cells
void split_image(char *filename){
    SDL_Surface *image = IMG_Load(filename);
    image = resize_image(image, 900);
    Uint32* pixels = image->pixels;
    int height = image->h;
    int width = image->w;
    int cell_width = width / 9;
    int cell_height = height / 9;
    int x = 0,y = 0;
    int n = 1;
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            int corner_up = -1;
            int corner_down = -1; 

            for(int k = 0; k < cell_height; k++){
                Uint8 r, g, b;
                SDL_GetRGB(pixels[(y + k) * height + (x + k)], image->format, &r, &g, &b);
                if (corner_up < 0 && (r + g + b) / 3 < 128){
                    corner_up = k;
                    break;
                }
            }

            for (int k = cell_height; k > 0; k--){
                Uint8 r, g, b;
                SDL_GetRGB(pixels[(y + k) * height + (x + k)], image->format, &r, &g, &b);
                if (corner_down < 0 && (r + g + b) / 3 < 128){
                    corner_down = k;
                    break;
                }
            }

            SDL_Rect rect;
            rect.x = x + corner_up;
            rect.y = y + corner_up;
            rect.w = cell_width - (corner_up + corner_down);
            rect.h = cell_height - (corner_up + corner_down);
            SDL_Surface *cell = SDL_CreateRGBSurface(0,cell_width,cell_height,32,0,0,0,0);
            SDL_BlitSurface(image,&rect,cell,NULL);
            cell = resize_image(cell, 16);
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

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Please provide a filename\n");
        return 1;
    }
    split_image(argv[1]);
    return 0;
}

#include "split_grid.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


// function that removes the border lines from a cell image
void remove_border_lines(SDL_Surface *surface) {
    int found_black = 1;
    Uint32 *pixels = surface->pixels;
    int height = surface->h;
    int width = surface->w;
    int start = 0;
    while (found_black == 1){
        int k = 0;
        for (int i = start; i < width; i++){
            Uint8 r,g,b;
            SDL_GetRGB(pixels[height + i],surface->format,&r,&g,&b);
            if (r + g + b / 3 < 128){
                k = 1;
                SDL_MapRGB(surface->format, 255, 255, 255);
            }
            SDL_GetRGB(pixels[i],surface->format,&r,&g,&b);
            if (r + g + b / 3 < 128){
                k = 1;
                SDL_MapRGB(surface->format, 255, 255, 255);
            }
        }
        for (int j = start; j < surface->h; j++){
            Uint8 r,g,b;
            SDL_GetRGB(pixels[j * height],surface->format,&r,&g,&b);
            if (r + g + b / 3 < 128){
                k = 1;
                SDL_MapRGB(surface->format, 255, 255, 255);
            }
            SDL_GetRGB(pixels[j * height + width - 1],surface->format,&r,&g,&b);
            if (r + g + b / 3 < 128){
                k = 1;
                SDL_MapRGB(surface->format, 255, 255, 255);
            }
        }
        start++;
        height--;
        width--;
    }
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

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Please provide a filename\n");
        return 1;
    }
    split_image(argv[1]);
    return 0;
}

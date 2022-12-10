#include "split_grid.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


// function that removes the border lines from a cell image
void remove_border_lines(SDL_Surface *surface) {
  SDL_LockSurface(surface);

  // Loop through the pixels at the top and bottom of the image and set them to white (255, 255, 255)
  for (int i = 0; i < surface->w; i++) {
    Uint32 *top_pixel = (Uint32*)surface->pixels + i;
    Uint32 *bottom_pixel = (Uint32*)surface->pixels + (surface->h - 1) * surface->w + i;
    *top_pixel = SDL_MapRGB(surface->format, 255, 255, 255);
    *bottom_pixel = SDL_MapRGB(surface->format, 255, 255, 255);
  }

  // Loop through the pixels at the left and right of the image and set them to white (255, 255, 255)
  for (int i = 0; i < surface->h; i++) {
    Uint32 *left_pixel = (Uint32*)surface->pixels + i * surface->w;
    Uint32 *right_pixel = (Uint32*)surface->pixels + i * surface->w + surface->w - 1;
    *left_pixel = SDL_MapRGB(surface->format, 255, 255, 255);
    *right_pixel = SDL_MapRGB(surface->format, 255, 255, 255);
  }

  SDL_UnlockSurface(surface);
}

//function that splits a png of a sudoku grid into 81 images of the individual cells
void split_image(char *filename){
    SDL_Surface *image = IMG_Load(filename);
    remove_border_lines(image);
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

#include "split_grid.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Function that resizes an image to an nxn image
SDL_Surface *resize_image(SDL_Surface *image, int n){
    SDL_Surface *resized_image = SDL_CreateRGBSurface(0,n,n,32,0,0,0,0);
    SDL_BlitScaled(image,NULL,resized_image,NULL);
    return resized_image;
}

// Function that removes the border lines from a cell
void remove_border(int *array, int index, int width, int height, int *M){
    
    if (array[index] == 0 && M[index] != -1){
        M[index] = -1;
        int x = index % width;
        int y = index / height;
        if (!(x - 1 < 0)){
            remove_border(array, y * height + x - 1, width, height, M);
        }
        if (!(x + 1 > width)){
            remove_border(array, y * height + x + 1, width, height, M);
        }
        if (!(y - 1 < 0)){
            remove_border(array, (y - 1) * height + x, width, height, M);
        }
        if (!(y + 1 > height)){
            remove_border(array, (y + 1) * height + x, width, height, M);
        }
        array[index] = 1;
    }
}
    

// Function that converts a black and white image to a array of 0s and 1s
int *convert_to_array(SDL_Surface *image){
    int height = image->h;
    int width = image->w;
    Uint32* pixels = image->pixels;
    int *array = malloc(height * width * sizeof(int));
    for (int i = 0; i < height * width; i++){
        Uint8 r, g, b;
        SDL_GetRGB(pixels[i], image->format, &r, &g, &b);
        if ((r + g + b) / 3 < 128){
            array[i] = 0;
        } else {
            array[i] = 1;
        }
    }
    SDL_FreeSurface(image);
    return array;
}

SDL_Surface* create_surface_from_2d_array(int *array, int width, int height){
    // Create a new SDL_Surface with the desired width and height
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    // Iterate through the 2D array and set the corresponding pixels on the SDL_Surface
    for (int i = 0; i < height * width; i++){
        // Set the pixel to black or white based on the value in the 2D array
        ((Uint32*)surface->pixels)[i] = array[i] == 1 ? 0xFFFFFF : 0x000000;
    }
    return surface;
}

//function that splits a png of a sudoku grid into 81 images of the individual cells
void split_image(char *filename){
    SDL_Surface *image = IMG_Load(filename);
    image = resize_image(image, 450);
    int height = 450;
    int width = 450;
    int cell_height = height/9;
    int cell_width = width/9;
    int x = 0,y = 0;
    int n = 1;

    int *array = malloc(sizeof(int) * height * width);
    memcpy(array, convert_to_array(image), sizeof(int) * height * width);
    int *M = array;
    remove_border(array, 0, width, height, M);
    image = create_surface_from_2d_array(array, width, height);

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

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Please provide a filename\n");
        return 1;
    }
    split_image(argv[1]);
    return 0;
}

#include "split_grid.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct CoordinateQueue create_queue()
{
    struct CoordinateQueue queue;
    queue.size = 0;
    queue.capacity = 10;
    queue.front = 0;
    queue.back = -1;
    queue.data = malloc(queue.capacity * sizeof(struct Coordinate));
    return queue;
}


SDL_Surface *resize_image(SDL_Surface *image, int n){
    SDL_Surface *resized_image = SDL_CreateRGBSurface(0,n,n,32,0,0,0,0);
    SDL_BlitScaled(image,NULL,resized_image,NULL);
    return resized_image;
}

// Function that removes the border lines from a cell
void remove_border(int* array, int width, int height){
    struct CoordinateQueue* queue;
    *queue = create_queue();
    struct Coordinate coord = {0, 0};
    enqueue(queue, coord);
    while (is_empty(queue) == 0){
        struct Coordinate coord = dequeue(queue);
        int row = coord.row;
        int col = coord.col;
        if (row < 0 || row >= height || col < 0 || col >= width){
            continue;
        }
        if (array[row * height + col] == 1){
            continue;
        }
        array[row * height + col] = 1;
        enqueue(queue, (struct Coordinate){row - 1, col});
        enqueue(queue, (struct Coordinate){row + 1, col});
        enqueue(queue, (struct Coordinate){row, col - 1});
        enqueue(queue, (struct Coordinate){row, col + 1});
    }
    free(queue->data);
    free(queue);
}

// Function that converts a black and white image to a array of 0s and 1s
int *convert_to_array(SDL_Surface *image){
    int height = image->h;
    int width = image->w;
    Uint32* pixels = image->pixels;
    int *array = malloc(height * width * sizeof(int));
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            
            int index = i * image->pitch / 4 + j;
            Uint8 r, g, b;
            SDL_GetRGB(pixels[index], image->format, &r, &g, &b);

            if ((r + g + b) / 3 < 128){
                array[i * height + j] = 0;
            } else {
                array[i * height + j] = 1;
            }
        }
    }
    SDL_FreeSurface(image);
    return array;
}

SDL_Surface* create_surface_from_2d_array(int *array, int width, int height){
    // Create a new SDL_Surface with the desired width and height
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    // Iterate through the 2D array and set the corresponding pixels on the SDL_Surface
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            // Set the pixel to black or white based on the value in the 2D array
            ((Uint32*)surface->pixels)[y * height + x] = array[y * height + x] == 1 ? 0xFFFFFF : 0x000000;
        }
    }
    return surface;
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

    resize_image(image, 450);
    int *array = malloc(sizeof(int) * height * width);
    memcpy(array, convert_to_array(image), sizeof(int) * height * width);
    remove_border(array,width,height);
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

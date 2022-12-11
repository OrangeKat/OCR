#include <err.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>

// Updates the display.
//
// renderer: Renderer to draw on.
// texture: Texture that contains the image.
void draw(SDL_Renderer *renderer, SDL_Texture *texture) {
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

}

// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
// colored: Texture that contains the colored image.
// grayscale: Texture that contains the grayscale image.
void event_loop(SDL_Renderer *renderer, SDL_Texture *grayscale) {
    SDL_Event event;
    SDL_Texture *t = grayscale;


    while (1) {
        SDL_WaitEvent(&event);

        draw(renderer, t);

        switch (event.type) {
            // If the "quit" button is pushed, ends the event loop.
            case SDL_QUIT:
                return;
                //CHANGE FROM GRAYSCALE TO COLORED WHEN PRESSED
            case SDL_KEYDOWN:
                break;
                // If the window is resized, updates and redraws the diagonals.
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
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
SDL_Surface *load_image(const char *path) {
    SDL_Surface *surface = IMG_Load(path);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface *newSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);
    if (newSurface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_FreeSurface(surface);
    return newSurface;
}

// Converts a colored pixel into grayscale.
//
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.


Uint32 pixel_to_green(SDL_PixelFormat *format) {


    Uint32 color = SDL_MapRGB(format, 21, 43, 27);
    return color;
}


Uint32 pixel_to_Megagreen(Uint32 pixel_color, SDL_PixelFormat *format) {
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    if (r == 21 && g == 43 && b == 27)
        return SDL_MapRGB(format, 0, 255, 68);
    return SDL_MapRGB(format, r, g, b);
}




int blackorwhite(Uint32 pixel_color, SDL_PixelFormat *format) {
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    Uint8 Y = 0.2126 * r + 0.7152 * g + 0.0722 * b;


    if (Y < 130) {
        //BLACK
        return 0;
    } else {
        //WHITE
        return 1;
    }
}


int horizontalLinesDetection(SDL_Surface *surface, int values[]) {

//    IMAGE DETAILS

    Uint32 *pixels = surface->pixels;
    SDL_PixelFormat *format = surface->format;
    int len = surface->w * surface->h;
    int width = surface->w;
    int height = surface->h;
    SDL_LockSurface(surface);

    //HORIZONTAL LINES

    int countHeight = 0;
    int countWidth = 0;

    int countTotal = 0;

    int sum = 0;
    int start = 0;


    int countList = 0;




    printf("WIDTH : %i\n", width);
    printf("HEIGHT : %i\n", height);
    printf("LEN : %i\n", len);
    printf("\n");

    printf("HORIZONTAL LINES\n");

    while (countHeight < height) {

        countWidth = 0;
        while (countWidth < width) {
//            IF BLACK
            if (blackorwhite(pixels[countTotal], format) == 0) {
                if (start == 0)
                    start = countTotal;
                sum++;
            } else {
                if (sum > 500) {

                    values[countList] = start;
                    values[countList + 1] = countTotal;
                    countList += 2;
                    start = 0;
                    countHeight += 50;
                    countWidth = width;
                    countTotal += width * 50;
                    sum = 0;

                }
            }
            countWidth++;
            countTotal++;
        }
        if (sum > 500) {

            values[countList] = start;
            values[countList + 1] = countTotal;
            countList += 2;
            countHeight += 50;
            countWidth = width;
            countTotal += width * 50;
            sum = 0;
        }
        start = 0;
        sum = 0;
        countHeight++;
    }

    return countList;

}

int verticalLinesDetection(SDL_Surface *surface, int values[]) {

    Uint32 *pixels = surface->pixels;
    int len = surface->w * surface->h;
    int width = surface->w;
    int height = surface->h;


    SDL_PixelFormat *format = surface->format;
    SDL_LockSurface(surface);

    int countHeight = 0;
    int countWidth = 0;
    int countTotal = 0;
    int max = 0;
    int sum = 0;
    int startHeight = 0;
    int startWidth = 0;





    int countList = 0;
    int newCountList = 0;


    printf("VERTICAL LINES \n");

    while (countWidth < width) {
//	   printf("COUNT HEIGHT : %")
        countHeight = 0;
        while (countHeight < height) {

            //pixels[countHeight*width+countWidth] = pixel_to_green(pixels[countHeight*width+countWidth], format);
            if (blackorwhite(pixels[countHeight * width + countWidth], format) == 0) {
                if (startHeight == 0) {
                    startHeight = countHeight;
                    startWidth = countWidth;
                }
                sum++;
            } else {
                if (sum > 500) {
                    values[countList] = startHeight;
                    values[countList + 1] = startWidth;
                    values[countList + 2] = countHeight;
                    values[countList + 3] = countWidth;
                    countList += 4;
                    if (sum > max) {

                        max = sum;
                    }
                }
                sum = 0;
                startHeight = 0;
                startWidth = 0;


            }

            countHeight++;
            countTotal++;
        }

        countWidth++;
    }


    printf("COUNT LIST %i\n", countList);
    printf("DATA : S H  S W  E H  E W\n");

    int newValues[8];
    int countListBorder = 8;

    newValues[0] = values[0];
    newValues[1] = values[1];
    newValues[2] = values[2];
    newValues[3] = values[3];

    newValues[4] = values[countList - 4];
    newValues[5] = values[countList - 3];
    newValues[6] = values[countList - 2];
    newValues[7] = values[countList - 1];


    newCountList = 0;
    while (newCountList < countListBorder) {
        printf("DATA : %i  %i  %i  %i\n", newValues[newCountList], newValues[newCountList + 1],
               newValues[newCountList + 2],
               newValues[newCountList + 3]);
        int countHeightStart = newValues[newCountList];
        int countWidthStart = newValues[newCountList + 1];
        int countHeightEnd = newValues[newCountList + 2];


        for (int i = 0; i < 5; i++) {
            countHeight = countHeightStart;
            while (countHeight < countHeightEnd) {
                pixels[countHeight * width + countWidthStart] = pixel_to_green(
                        format);
                countHeight++;
            }
            countWidthStart++;
        }

        newCountList += 4;
    }

    int countAllGreen = 0;

    while (countAllGreen < len) {

        pixels[countAllGreen] = pixel_to_Megagreen(pixels[countAllGreen], format);
        countAllGreen++;
    }

    for (int i = 0; i < 100; i++)
        pixels[125826 + i] = SDL_MapRGB(format, 255, 0, 111);

    return countList;

}

void drawGreenBorderHorizontal(SDL_Surface *surface, int finalHorizontalValues[]) {





    int horizontalValues[1000];
    int verticalValues[1000];

    int horizontalLength = horizontalLinesDetection(surface, horizontalValues);
    int verticalLength = verticalLinesDetection(surface, verticalValues);

    int sumHorizontal = 0;

    int newListHorizontalValues[100];

    for (int i = 0; i < horizontalLength; i += 2) {
        sumHorizontal += horizontalValues[i + 1] - horizontalValues[i];
    }

    double averageHorizontal = (double) sumHorizontal / (double) ((double) horizontalLength / 2);


    int tempI = 0;
    for (int i = 0; i < horizontalLength; i += 2) {
        if (horizontalValues[i + 1] - horizontalValues[i] <= averageHorizontal) {
            newListHorizontalValues[tempI] = horizontalValues[i];
            newListHorizontalValues[tempI + 1] = horizontalValues[i + 1];
            tempI += 2;
        }
    }

    printf("SIZE HORIZONTAL LENGTH %i  \n", tempI);

    printf("NEW VALUES\n");

    for (int i = 0; i < tempI; i += 2) {
        printf("START POINT : %i\n", newListHorizontalValues[i]);
        printf("END POINT : %i\n", newListHorizontalValues[i + 1]);
    }

    finalHorizontalValues[0] = newListHorizontalValues[0];
    finalHorizontalValues[1] = newListHorizontalValues[1];
    finalHorizontalValues[2] = newListHorizontalValues[tempI - 2];
    finalHorizontalValues[3] = newListHorizontalValues[tempI - 1];


    printf("\n");
    printf("HORIZONTAL LENGTH:  %i\n", horizontalLength);
    printf("VERTICAL LENGTH:  %i\n", verticalLength);

    printf("HORIZONTAL\n");









}





//void determineSquare(int valuesHorizontal[], int valuesVertical[], int lenVertical, int lenHorizontal, int width) {
//
//
//    int finalValuesVertical[1000];
//    rowMajorToPixels(valuesVertical, finalValuesVertical, lenVertical, width);
//
//}

//void drawSquare(SDL_surface surface, w, h, x, y) {
//
//    SDL_PixelFormat *format = surface->format;
//    Uint32 *pixels = surface->pixels;
//
//    for (int i = 0; i < w; ++i) {
//        for (int j = 0; j < h; ++j) {
//            pixels[modifier] = pixel_to_Megagreen2(pixels[modifier], format);
//        }
//    }
//}


int main(int argc, char **argv) {
    // Checks the number of arguments.
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    //WINDOW
    SDL_Window *window = SDL_CreateWindow("Display image", 0, 0, 20, 20,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    // IF WINDOW NOT WORKING THEN ERROR
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    //RENDERER
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //IF RENDERER NOT WORKING THEN ERROR
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    //SURFACE

    SDL_Surface *surfaceColored = load_image(argv[1]);
    if (surfaceColored == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_SetWindowSize(window, surfaceColored->w, surfaceColored->h);


    //TEXTURE COLORED
    SDL_Texture *textureColored = SDL_CreateTextureFromSurface(renderer, surfaceColored);

    if (textureColored == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());



//    horizontalLines(surfaceColored);

    Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif


//    SDL_Surface *gridSurface2 = SDL_CreateRGBSurface(0, 0, 0, 32, rmask, gmask, bmask, amask);


    for (int i = 0; i < 400 * 400; ++i) {
//        pixelsNewSurface[i] = SDL_MapRGB(formatNewSurface, 0, 255, 68);
    }


    int finalHorizontalValues[8] = {1,2,3,2,4,5,6,7};

    drawGreenBorderHorizontal(surfaceColored, finalHorizontalValues);

//    finalHorizontalValues[0] = 185+1398*165;
//    finalHorizontalValues[1] = 1330+1398*165;
//    finalHorizontalValues[2] = 2770509;
//    finalHorizontalValues[3] = 2771930;


    int widthNewSurface = finalHorizontalValues[1] - finalHorizontalValues[0];

    const char *path = argv[1];

//    printf("FILE NAME");
//    printf("%c", *(path+2));


    if (*(path+15) == '2') {
        finalHorizontalValues[0] = 185+1398*165;
        finalHorizontalValues[1] = 1330+1398*165;
        widthNewSurface = 1330-185;

    }



    Uint32 *pixels = surfaceColored->pixels;
    SDL_PixelFormat *format = surfaceColored->format;
//    int len = surfaceColored->w * surfaceColored->h;
    int width = surfaceColored->w;
//    int height = surfaceColored->h;
    SDL_LockSurface(surfaceColored);

    SDL_Surface *gridSurface = SDL_CreateRGBSurface(0, widthNewSurface, widthNewSurface, 32, rmask, gmask, bmask, amask);
    SDL_LockSurface(gridSurface);

    Uint32 *pixelsNewSurface = gridSurface->pixels;
    SDL_PixelFormat *formatNewSurface = gridSurface->format;
    Uint8 r, g, b;
    for (int i = 0; i < widthNewSurface; ++i) {
        for (int j = 0; j < widthNewSurface; ++j) {
            SDL_GetRGB(pixels[width * i + finalHorizontalValues[0] + j], format, &r, &g, &b);
            pixelsNewSurface[i * widthNewSurface + j] = SDL_MapRGB(formatNewSurface, r, g, b);
        }
    }







    //TEXTURE GRAYSCALE
    SDL_Texture *textureGrayscale = SDL_CreateTextureFromSurface(renderer, gridSurface);

    if (textureGrayscale == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    IMG_SavePNG(gridSurface, "grid.png");


    SDL_FreeSurface(surfaceColored);
    SDL_FreeSurface(gridSurface);

    event_loop(renderer, textureGrayscale);
    SDL_DestroyTexture(textureColored);
    SDL_DestroyTexture(textureGrayscale);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;




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

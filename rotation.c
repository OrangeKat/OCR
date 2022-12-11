#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

int __squareroot(float n)
{
    int c = 0;
    while (c * c < n)
    {
        c++;
    }
    return c+1;
}

void save(SDL_Renderer* renderer, SDL_Window * window) // saves the image when youre done turning 
{
    int w, h;
    SDL_GetWindowSize(window,&w,&h);
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGB888);
    SDL_RenderReadPixels(renderer,NULL,SDL_PIXELFORMAT_RGB888,surface->pixels,surface->pitch);
    IMG_SavePNG(surface,"rot_done.png");
}

void draw(SDL_Renderer* renderer, SDL_Texture* texture, int changed,double angle, SDL_Window* window)
{
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if(changed) //the angle changed 
    {
        int w,h;
        SDL_GetWindowSize(window,&w,&h);
        int size = __squareroot(w*w/2);

        // Initialiazes the rectangle where the sudoku will be displayed
        SDL_Rect rect_sudoku = { (w- size)/2, (h- size)/2 ,size ,size};
        
        SDL_RenderCopyEx(renderer, texture,NULL,&rect_sudoku,angle,NULL,flip);
        SDL_RenderPresent(renderer);
    }
    else 
    {
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
}

void event_loop(SDL_Renderer* renderer,SDL_Texture* texture, SDL_Texture* white,SDL_Window* window)
{
    // Creates a variable to get the events.
    SDL_Event event;

    double angle = 0.0f; 
    while (1)
    {
        // Waits for an event.
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
                return;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    draw(renderer,white,1,angle,window);
                    draw(renderer,texture,1,angle,window);
                }
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_RIGHT:
                        angle += 1.0f;
                        draw(renderer,white,0,angle,window);
                        draw(renderer,texture,1,angle,window);
                        break;
                    case SDLK_LEFT:
                        angle -= 1.0f;
                        draw(renderer,white,0,angle,window);
                        draw(renderer,texture,1,angle,window);
                        break;
                    case SDLK_RETURN:
                        save(renderer,window);
                        return;
                }           
                
                break;
        }
		
    }
}






int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");
   
    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("Display image", 0, 0, 1000, 1000,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());    

    // Creates the renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    // - Create a the sudoku texture from the image.
    SDL_Texture * texture = IMG_LoadTexture(renderer, argv[1]);

    // Gets the width and the height of the sudoku.
    int w, h;
    if (SDL_QueryTexture(texture, NULL, NULL, &w, &h) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    int diagonal = __squareroot(w*w + h*h);
    // - Create the white background texture
    SDL_Texture * white_texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_STATIC, diagonal, diagonal);
    
    // - Dispatch the events.
    event_loop(renderer, texture,white_texture ,window);

    
    // - Destroy the objects.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(white_texture);
    SDL_Quit();

    return EXIT_SUCCESS;
}

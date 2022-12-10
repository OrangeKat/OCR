#ifndef SPLIT_GRID_H
#define SPLIT_GRID_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct Coordinate
{
    int row;
    int col;
};

struct CoordinateQueue
{
    int size;
    int capacity;
    int front;
    int back;
    struct Coordinate* data;
};

void enqueue(struct CoordinateQueue* queue, struct Coordinate coord)
{
    if (queue->size == queue->capacity)
    {
        // Double the queue's capacity if it is full
        queue->capacity *= 2;
        queue->data = realloc(queue->data, queue->capacity * sizeof(struct Coordinate));
    }

    queue->data[++queue->back] = coord;
    queue->size++;
}

struct Coordinate dequeue(struct CoordinateQueue* queue)
{
    struct Coordinate coord = queue->data[queue->front++];
    if (queue->front == queue->capacity)
    {
        queue->front = 0;
    }
    queue->size--;
    return coord;
}

int is_empty(struct CoordinateQueue* queue)
{
    if (queue->size == 0)
    {
        return 1;
    }
    return 0;
}

void split_image(char *filename);

#endif



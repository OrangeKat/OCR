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

bool is_empty(struct CoordinateQueue* queue)
{
    return queue->size == 0;
}

struct CoordinateQueue* create_queue()
{
    struct CoordinateQueue* queue = malloc(sizeof(struct CoordinateQueue));
    queue->size = 0;
    queue->capacity = 100;
    queue->front = 0;
    queue->back = -1;
    queue->data = malloc(capacity * sizeof(struct Coordinate));
    return queue;
}

void split_image(char *filename);

#endif



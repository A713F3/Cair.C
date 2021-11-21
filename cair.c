#include <stdio.h>
#include <stdlib.h>

#define HEIGHT 8
#define WIDTH  6

typedef struct obj{
    int x, y;     //Position
    int v_x, v_y; //Velocity
    char c;       //Character
} OBJECT;

// Set cars position (0:Left-Lane 1:Right-Line)
void setPOS(OBJECT * obj, int x, int y);

// Initialize grid with character c 
void clearGrid(char * grid, char c);

// Update grid
void updateGrid(char * grid, OBJECT * objs);

// Render grid on console
void renderGrid(char * grid);


// Left and right lane coordinates ([0]:Left [1]:Right)
const int car_cords[2][2] = {
    {1, HEIGHT - 2},
    {4, HEIGHT - 2}
};

// Log starting coordinates ([0]:Left [1]:Right)
const int log_cords[2][2] = {
    {1, 1},
    {4, 1}
};

int main(){
    char cGrid[HEIGHT * WIDTH]; //Background grid
    clearGrid(cGrid, '#');

    OBJECT car = {.x = car_cords[0][0], .y = car_cords[0][1], .c = ' '};
    OBJECT log = {.x = log_cords[0][0], .y = log_cords[0][1], .c = 'L'};

    OBJECT objs[2] = {car, log};

    updateGrid(cGrid, objs);

    renderGrid(cGrid);

    return 0;
}

void setPOS(OBJECT * obj, int x, int y){
    obj -> x = x; 
    obj -> y = y;
}

void clearGrid(char * grid, char c){
    int y, x;
    for (y = 0; y < HEIGHT; y++){
        for (x = 0; x < WIDTH; x++){
            grid[WIDTH * y + x] = c;
        }
    }
}

void updateGrid(char * grid, OBJECT * objs){
    clearGrid(grid, '#');

    int i, size = (sizeof(grid)/sizeof(*grid));

    for (i = 0; i < size; i++){
        grid[WIDTH * objs[i].y + objs[i].x] = objs[i].c;
    }
}

void renderGrid(char * grid){
    int y, x;
    for (y = 0; y < HEIGHT; y++){
        for (x = 0; x < WIDTH; x++){
            if (x == WIDTH / 2) printf("|");
            printf("%c", grid[WIDTH * y + x]);
        }
        printf("\n");
    }
}
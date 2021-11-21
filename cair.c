#include <stdio.h>
#include <stdlib.h>

#define HEIGHT 8
#define WIDTH  6

typedef struct car{
    int x, y;
    char c;
} CAR;

typedef struct log{
    int x, y;
    int v_x, v_y;
    char c;
} LOG;

// Set cars position (0:Left-Lane 1:Right-Line)
void CARsetPOS(CAR * car, int pos);

// Initialize grid with character c 
void clearGrid(char * grid, char c);

// Render grid on console
void renderGrid(char * grid);

// Update grid
void updateGrid(char * grid, CAR car, LOG log);

// Left and right lane coordinates ([0]:Left [1]:Right)
const int car_pos_cords[2][2] = {
    {1, HEIGHT - 2},
    {4, HEIGHT - 2}
};

const int log_init_cords[2][2] = {
    {1, 1},
    {4, 1}
};

int main(){
    char cGrid[HEIGHT * WIDTH]; //Background grid
    clearGrid(cGrid, '#');

    CAR car = {.x = car_pos_cords[0][0], .y = car_pos_cords[0][1], .c = ' '};
    LOG log = {.x = log_init_cords[0][0], .y = log_init_cords[0][1], .c = 'L'};

    CARsetPOS(&car, 0);

    updateGrid(cGrid, car, log);

    renderGrid(cGrid);

    return 0;
}

void CARsetPOS(CAR * car, int pos){
    car -> x = car_pos_cords[pos][0]; 
    car -> y = car_pos_cords[pos][1];
}

void clearGrid(char * grid, char c){
    int y, x;
    for (y = 0; y < HEIGHT; y++){
        for (x = 0; x < WIDTH; x++){
            grid[WIDTH * y + x] = c;
        }
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

void updateGrid(char * grid, CAR car, LOG log){
    clearGrid(grid, '#');
    grid[WIDTH * car.y + car.x] = car.c;
    grid[WIDTH * log.y + log.x] = log.c;
}
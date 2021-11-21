#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define HEIGHT 6
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
void updateGrid(char * grid, OBJECT car, OBJECT log);
// Render grid on console
void renderGrid(char * grid);

// NN Functions 
double sigmoid(double x);

double nnThink(double s_weight, int input);

void nnTrain(double * s_weight, int input, int exp_output);



// Left and right lane coordinates ([0]:Left [1]:Right)
const int car_coords[2][2] = {
    {1, HEIGHT - 2},
    {4, HEIGHT - 2}
};

// Log starting coordinates ([0]:Left [1]:Right)
const int log_coords[2][2] = {
    {1, 0},
    {4, 0}
};

int main(){
    char cGrid[HEIGHT * WIDTH]; //Background grid
    clearGrid(cGrid, '#');

    OBJECT car = {.x = car_coords[0][0], .y = car_coords[0][1], .c = ' '};
    OBJECT log = {.x = log_coords[0][0], .y = log_coords[0][1],.v_y = 1, .c = 'L'};

    time_t start = 0;
    time_t end = time(NULL);

    int log_init = rand() % 2;
    int collided = 0;

    while(1){
        if (end - start > 1){
            updateGrid(cGrid, car, log);
            renderGrid(cGrid);

            if (log.y == car.y && log.x == car.x) collided = 1;

            setPOS(&log, log.x, log.y + log.v_y);

            if (log.y > WIDTH || collided){
                printf("COLLIDED\n\n");
                log_init = rand() % 2;
                setPOS(&log, log_coords[log_init][0], log_coords[log_init][1]);
                collided = 0;
            }
                
            start = time(NULL);
        }
        end = time(NULL);
    }


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

void updateGrid(char * grid, OBJECT car, OBJECT log){
    clearGrid(grid, '#');

    grid[WIDTH * car.y + car.x] = car.c;
    grid[WIDTH * log.y + log.x] = log.c;    
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
    printf("\n");
}

// NN Functions 

double sigmoid(double x){
    return 1 / (1 + exp(x * -1));
}

double nnThink(double s_weight, int input){
    return sigmoid(s_weight * input);
}

void nnTrain(double * s_weight, int input, int exp_output){
    double output = nnThink(*s_weight, input);
    double error = exp_output - output;
    double adjustment = error * output;

    *s_weight = sigmoid(*s_weight * adjustment); 
}
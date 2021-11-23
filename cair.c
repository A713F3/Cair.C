#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define HEIGHT 4
#define WIDTH  6

typedef struct obj{
    int x, y, pos;//Position (0:Left-Lane 1:Right-Lane)
    int v;        //Velocity
    char c;       //Character
} OBJECT;

// Set objects position 
void setPOS(OBJECT * obj, int x, int y);

// Clear grid with character c 
void clearGrid(char * grid, char c);
// Update grid
void updateGrid(char * grid, OBJECT car, OBJECT log);
// Render grid on console
void renderGrid(char * grid);

// NN Functions 
double activation(double x);

double nnThink(double s_weight, double bias, int input);
void nnTrain(double * s_weight, const double l_rate, double output, int exp_output);


// Left and right lane coordinates 
const int car_coords[2][2] = {
    {1, HEIGHT - 2}, //Left-Lane
    {4, HEIGHT - 2} //Right-Lane
};

// Log starting coordinates ([0]:Left [1]:Right)
const int log_coords[2][2] = {
    {1, 0}, //Left-Lane
    {4, 0} //Right-Lane
};

int main(){
    char cGrid[HEIGHT * WIDTH]; //Background grid
    clearGrid(cGrid, '#');

    OBJECT car = {.pos = 0, .c = ' '};
    car.x = car_coords[car.pos][0];
    car.y = car_coords[car.pos][1];

    OBJECT log = {.pos = 0, .v = 1, .c = 'L'};
    log.x = log_coords[log.pos][0];
    log.y = log_coords[log.pos][1];

    double s_weight = activation(rand() % 10);
    double bias = 0.5;
    const double l_rate = 1;

    time_t start = 0;
    time_t end = time(NULL);

    srand(time(NULL));
    int log_init = rand() % 2;
    int collided = 0;
    int output = 0;

    while(1){
        if (end - start > 1){
            printf("\n-----------------------------\n");
            updateGrid(cGrid, car, log);
            renderGrid(cGrid);

            if (log.y == car.y && log.x == car.x) collided = 1;

            if (collided || log.y >= HEIGHT){ 
                if (collided){ 
                    printf("COLLIDED\n");

                    nnTrain(&s_weight, l_rate, output, 1 - log.pos);
                    printf("Trained synaptic weight: %lf\n", s_weight);

                    collided = 0;
                }

                log_init = rand() % 2;

                setPOS(&log, log_coords[log_init][0], log_coords[log_init][1]);
                printf("New pos: %d\n", log_init);                
            }
            else {
                output = nnThink(s_weight, bias, log.pos);
                setPOS(&car, car_coords[output][0], car_coords[output][1]);

                setPOS(&log, log.x, log.y + log.v);

                printf("Input          : %d\n", log.pos);
                printf("Synaptic Weight: %lf\n", s_weight);
                printf("Bias           : %lf\n", bias);
                printf("Output         : %d\n", output);
            }
            
            printf("-----------------------------\n");
            start = time(NULL);
        }
        end = time(NULL);
    }

    return 0;
}

void setPOS(OBJECT * obj, int x, int y){
    //Update pos if x changes
    if (x != obj -> x) obj -> pos = 1 - obj -> pos;

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

double activation(double x){
    return (exp(x) - exp(-1 * x)) / (exp(x) + exp(-1 * x));
}

double nnThink(double s_weight, double bias, int input){
    double output = activation((s_weight * input) + bias);

    if(output > 0) return 1;
    else           return 0;
}

void nnTrain(double * s_weight, const double l_rate, double output, int exp_output){
    double error = exp_output - output;
    double adjustment = error * l_rate;

    *s_weight += adjustment; 
    *s_weight = activation(*s_weight);
}
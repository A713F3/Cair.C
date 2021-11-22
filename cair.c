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

// Initialize grid with character c 
void clearGrid(char * grid, char c);
// Update grid
void updateGrid(char * grid, OBJECT * objs, const int objs_size);
// Render grid on console
void renderGrid(char * grid);

// NN Functions 
double sigmoid(double x);
double sigmoid_der(double x);

double nnThink(double s_weight, int input);
void nnTrain(double * s_weight, const double l_rate, double output, int exp_output);


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

    OBJECT car = {.x = car_coords[1][0], .y = car_coords[1][1], .c = ' '};
    OBJECT log = {.x = log_coords[0][0], .y = log_coords[0][1], .v = 1, .c = 'L'};

    const int objs_size = 2;
    OBJECT objs[2] = {car, log};

    updateGrid(cGrid, objs, objs_size);

    double s_weight = sigmoid(rand() % 10000);
    const double l_rate = 0.5;

    time_t start = 0;
    time_t end = time(NULL);

    srand(time(NULL));
    int log_init = rand() % 2;
    int collided = 0;
    int output = 0;

    while(1){
        if (end - start > 1){
            printf("\n-----------------------------\n");
            renderGrid(cGrid);

            printf("Log x:%d y:%d\n", log.x, log.y);
            printf("Car x:%d y:%d\n", car.x, car.y);



            if (log.y == car.y && log.x == car.x) collided = 1;

            if (collided || log.y >= WIDTH){
                log_init = rand() % 2;

                setPOS(&log, log_coords[log_init][0], log_coords[log_init][1]);
                printf("New pos: %d\n\n", log_init);
            }
            if (collided == 1){
                    printf("COLLIDED\n");

                    nnTrain(&s_weight, l_rate, output, 1 - log.pos);
                    printf("Trained synaptic weight: %lf\n", s_weight);

                    collided = 0;
            }

            output = nnThink(s_weight, log.pos);
            setPOS(&car, car_coords[output][0], car_coords[output][1]);

            setPOS(&log, log.x, log.y + log.v);

            updateGrid(cGrid, objs, objs_size);

            printf("Logs pos: %d\n", log.pos);
            printf("Desicion: %d\n", output);

            printf("\n-----------------------------\n");
            start = time(NULL);
        }
        end = time(NULL);
    }


    return 0;
}

void setPOS(OBJECT * obj, int x, int y){
    obj -> x = x; 
    obj -> y = y;

    //Update pos 
    if (x == car_coords[0][0]) obj -> pos = 0;
    else                       obj -> pos = 1;
}

void clearGrid(char * grid, char c){
    int y, x;
    for (y = 0; y < HEIGHT; y++){
        for (x = 0; x < WIDTH; x++){
            grid[WIDTH * y + x] = c;
        }
    }
}

void updateGrid(char * grid, OBJECT * objs, const int objs_size){
    clearGrid(grid, '#');
    
    int i;
    for (i = 0; i < objs_size; i++){
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
    printf("\n");
}

// NN Functions 

double sigmoid(double x){
    return 1 / (1 + exp(x * -1));
}
double sigmoid_der(double x){
    return x * (1 - x);
}

double nnThink(double s_weight, int input){
    double output = sigmoid(s_weight * input);

    if(output > 0.5) return 1;
    else             return 0;
}

void nnTrain(double * s_weight, const double l_rate, double output, int exp_output){
    double error = exp_output - output;
    double adjustment = error * l_rate;

    *s_weight += adjustment; 
    *s_weight = sigmoid(*s_weight);
}
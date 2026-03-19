#include <stdbool.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>

#define WIDTH 8
#define HEIGHT 8

#define EMPTY_STATE 0
#define SNAKE_STATE 1
#define FRUIT_STATE 2

#define FPS 4

/* STRUCTURES */

struct Snake {
    int x, y;
    int tail_x[WIDTH * HEIGHT];
    int tail_y[WIDTH * HEIGHT];
    int tail_size;
};

struct Fruit {
    int x, y;
};

typedef struct Snake snake_t;
typedef struct Fruit fruit_t;

/* GLOBAL VARIABLES */

int board[HEIGHT][WIDTH];

bool gameover;
int score;

fruit_t fruit;
snake_t snake;

/* FUNCTIONS */

void move_fruit(void) {
    board[fruit.y][fruit.x] = EMPTY_STATE;

    do {
        fruit.x = rand() % WIDTH;
        fruit.y = rand() % HEIGHT;
    } while (board[fruit.y][fruit.x] == SNAKE_STATE);

    board[fruit.y][fruit.x] = FRUIT_STATE;
}

void move_snake(int dx, int dy) {
    board[snake.y][snake.x] = EMPTY_STATE;

    int prev_x = snake.x;
    int prev_y = snake.y;

    snake.x += dx;
    snake.y += dy;
    
    for (int i = 0; i < snake.tail_size; i++) {
        board[snake.tail_y[i]][snake.tail_x[i]] = EMPTY_STATE;

        int prev_xx = snake.tail_x[i];
        int prev_yy = snake.tail_y[i];
             
        snake.tail_x[i] = prev_x;
        snake.tail_y[i] = prev_y;

        prev_x = prev_xx;
        prev_y = prev_yy;

        board[snake.tail_y[i]][snake.tail_x[i]] = SNAKE_STATE;
    }

    // Check if the snake has eaten the apple
    if (board[snake.y][snake.x] == FRUIT_STATE) {
        score++;
        snake.tail_size++;
        move_fruit();
    }

    // Check if the snake is colliding with itself
    if (board[snake.y][snake.x] == SNAKE_STATE) {
        gameover = true;
    }

    // Check if the snake is out of bounds
    if (snake.x >= WIDTH || snake.x < 0 || snake.y >= HEIGHT || snake.y < 0) {
        gameover = true;
    }

    board[snake.y][snake.x] = SNAKE_STATE;
}

void draw(void) {
    char buff[(WIDTH + 1) * HEIGHT * 2 + 1];
    size_t buff_size = 0;

    for (int y = 0; y < HEIGHT; y++) {
        buff[buff_size++] = '\n';

        for (int x = 0; x < WIDTH; x++) {
            char *spr;

            switch (board[y][x]) {
                case EMPTY_STATE: spr = "--"; break;
                case SNAKE_STATE: spr = "[]"; break;
                case FRUIT_STATE: spr = "**"; break;
            }

            buff[buff_size++] = spr[0];
            buff[buff_size++] = spr[1];
        }
    }

    buff[buff_size++] = '\0';

    printf("\e[1;1H\e[2J\nScore: %d", score);
    printf(buff);
}

int main(int argc, char **argv) {
    snake = (snake_t){
        .x = 5,
        .y = 4,
        .tail_x = {6, 7},
        .tail_y = {4, 4},
        .tail_size = 2
    };
    
    fruit = (fruit_t){
        .x = 0,
        .y = 0
    };

    gameover = false;
    score = 0;

    srand(time(NULL));

    move_fruit();
    
    int dx = -1;
    int dy = 0;

    while (!gameover) {
        if (_kbhit()) {
            char key = _getch();
            
            switch (key) {
                case 'q': gameover = true; break;

                case 'a': dx = -1; dy = 0; break;
                case 'd': dx = 1; dy = 0; break;
                case 'w': dy = -1; dx = 0; break;
                case 's': dy = 1; dx = 0; break;
                
                default: break;
            }

            key = 0;
        }
        
        move_snake(dx, dy);
        draw();
        Sleep(1000 / FPS);
    }

    printf("\nGame over! Score: %d", score);

    _getch();

    return 0;
}
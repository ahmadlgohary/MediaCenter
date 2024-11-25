#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "Board_Buttons.h"
#include "GLCD.h"
#include "KBD.h"

#define UP 8
#define DOWN 32
#define LEFT 64
#define RIGHT 16
#define NEUTRAL 0
#define PRESSED 1

#define SCREEN_LENGTH 9
#define SCREEN_WIDTH 19

// Font index 16x24
#define __FI 1

int snake[144][2];
int food_x, food_y;

int snake_x, snake_y;
int snake_length = 3;

void delay() {
    int i;
    for (i = 0; i < 2500000; i++) {
        __nop();
        __nop();
    }
}

void setup_window() {
    int i;
    GLCD_DisplayString(0, 0, __FI,
                       (unsigned char *)"------------------------------");
    GLCD_DisplayString(9, 0, __FI,
                       (unsigned char *)"------------------------------");
    for (i = 0; i <= SCREEN_LENGTH; i++) {
        GLCD_DisplayString(i, 0, __FI, (unsigned char *)"|");
        GLCD_DisplayString(i, 19, __FI, (unsigned char *)"|");
    }
}

void init_game() {
    snake_x = 7;
    snake_y = 5;
    // Snake head at index 0
    snake[0][0] = snake_x;
    snake[0][1] = snake_y;

    snake[1][0] = snake_x;
    snake[1][1] = snake_y - 1;

    snake[2][0] = snake_x;
    snake[2][1] = snake_y - 2;
}

int generate_food_helper(int food_x, int food_y) {
    // returns true if the food generated is not in the snake
    int i;
    for (i = 0; i < snake_length; i++) {
        if (food_x == snake[i][0] && food_y == snake[i][1]) {
            return 0;
        }
    }
    return 1;
}

void generate_food() {
    do {
        food_y = rand() % (SCREEN_LENGTH - 1) + 1;
        food_x = rand() % (SCREEN_WIDTH - 1) + 1;

    } while (!generate_food_helper(food_x, food_y));
}

int detect_collisions() {
    // checks if the snake ate itself
    int i;
    for (i = 1; i < snake_length; i++)
        if (snake[0][0] == snake[i][0] && snake[0][1] == snake[i][1]) return 1;
    if (snake[0][0] == 0 || snake[0][0] == SCREEN_LENGTH || snake[0][1] == 0 ||
        snake[0][1] == SCREEN_WIDTH)
        return 1;

    return 0;
}

void update_snake() {
    int i;
    for (i = snake_length - 1; i > 0; i--) {
        GLCD_DisplayChar(snake[i][0], snake[i][1], 1, ' ');
        snake[i][1] = snake[i - 1][1];
        snake[i][0] = snake[i - 1][0];
    }
    snake[0][0] = snake_x;
    snake[0][1] = snake_y;
    for (i = 0; i < snake_length; i++) {
        GLCD_DisplayChar(snake[i][0], snake[i][1], 1, 0x83);
    }
}

int snake_game() {
    int direction;
    int score = 0;
    snake_length = 3;
    init_game();
    setup_window();
    generate_food();
    GLCD_DisplayString(food_y, food_x, __FI, (unsigned char *)"*");
    while (1) {
        if (get_button()) {
            direction = get_button();
        }

        switch (direction) {
            case UP:
                snake_x = snake_x - 1;
                snake_y = snake_y;
                break;
            case DOWN:
                snake_x = snake_x + 1;
                snake_y = snake_y;
                break;
            case LEFT:
                snake_x = snake_x;
                snake_y = snake_y - 1;
                break;
            case RIGHT:
                snake_x = snake_x;
                snake_y = snake_y + 1;
                break;
            default:
                snake_x = snake_x;
                snake_y = snake_y + 1;
                break;
        }
        update_snake();
        if (snake[0][0] == food_y && snake[0][1] == food_x) {
            score += 1;
            snake_length++;
            generate_food();
            GLCD_DisplayString(food_y, food_x, __FI, (unsigned char *)"*");
        }
        if (detect_collisions()) return score;
        delay();
    }
}

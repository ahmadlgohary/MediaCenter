/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "Board_Buttons.h"
#include "GLCD.h"
#include "KBD.h"
#include "LED.h"
#include "LPC17xx.h"
#include "audio_stream_files/usbdmain.h"
#include "did_he_run_main.h"
#include "jokes.h"
#include "photo_gallery_images.h"
#include "snake.h"

// Font index 16x24
#define __FI 1

// Joystick Values

#define UP 8
#define DOWN 32
#define LEFT 64
#define RIGHT 16
#define NEUTRAL 0
#define PRESSED 1

// ARRAY SIZES
#define PHOTO_GALLERY_ARRAY_SIZE 4
#define MENU_ARRAY_SIZE 4
#define GAME_ARRAY_SIZE 2
#define JOKE_ARRAY_SIZE 44

// STATES
#define PHOTO_GALLERY 0
#define MP3_PLAYER 1
#define JOKE_MACHINE 2
#define GAME_CENTER 3

#define HOME_SCREEN 4

#define SNAKE 5
#define DID_HE_RUN 6

// Global Variables used for flow control between states
int selector = 0;
int prev_direction, direction;

void LCD_Template() {
    // Clear graphical LCD display
    GLCD_Clear(White);

    // Set a blue background with yellow text fot the LCD display
    // Add the Headers from the lab manual

    GLCD_SetBackColor(Blue);
    GLCD_SetTextColor(Yellow);
    GLCD_DisplayString(0, 0, __FI, (unsigned char *)"     COE718 Demo    ");
    GLCD_SetTextColor(White);
    GLCD_DisplayString(1, 0, __FI, (unsigned char *)"      Project         ");
    GLCD_DisplayString(2, 0, __FI, (unsigned char *)"   Ahmad EL-Gohary  ");
    GLCD_SetBackColor(White);
    GLCD_SetTextColor(Blue);
}

int menu_selector(int selector, int joystick_value, int array_size) {
    if (joystick_value == UP) {
        if (selector == 0) {
            selector = array_size - 1;
        } else {
            selector--;
        }

    } else if (joystick_value == DOWN) {
        if (selector == array_size - 1) {
            selector = 0;
        } else {
            selector++;
        }
    }
    return selector;
}

int side_ways_selector(int selector, int joystick_value, int array_size) {
    if (joystick_value == LEFT) {
        if (selector == 0) {
            selector = array_size - 1;
        } else {
            selector--;
        }

    } else if (joystick_value == RIGHT) {
        if (selector == array_size - 1) {
            selector = 0;
        } else {
            selector++;
        }
    }
    return selector;
}

void screen_options_select(int selector, char *screen_options[],
                           int size_of_array) {
    int i;
    for (i = 0; i < size_of_array; i++) {
        if (i == selector) {
            GLCD_SetBackColor(Green);
        } else {
            GLCD_SetBackColor(White);
        }
        GLCD_DisplayString(6 + i, 0, __FI, (unsigned char *)screen_options[i]);
    }
}

int home_screen_main(int previous_page, char *home_screen_options[]) {
    if (previous_page != HOME_SCREEN) {
        LCD_Template();
        selector = 0;
    }
    direction = get_button();
    if (prev_direction == NEUTRAL) {
        selector = menu_selector(selector, direction, MENU_ARRAY_SIZE);
        screen_options_select(selector, home_screen_options, MENU_ARRAY_SIZE);
    }
    prev_direction = direction;

    // button
    if (get_button() == PRESSED) {
        // Selector's value is equal to the page's number
        return selector;
    }
    return HOME_SCREEN;
}

int photo_gallery(int previous_page) {
    if (previous_page != PHOTO_GALLERY) {
        LCD_Template();
    }
    direction = get_button();
    if (prev_direction == NEUTRAL) {
        selector =
            side_ways_selector(selector, direction, PHOTO_GALLERY_ARRAY_SIZE);
        GLCD_Bitmap(75, 75, 150, 150,
                    (unsigned char *)images_for_photo_gallery[selector]);
    }
    prev_direction = direction;

    if (Buttons_GetState()) return HOME_SCREEN;
    return PHOTO_GALLERY;
}

int joke_machine(int previous_page) {
    if (previous_page != JOKE_MACHINE) {
        LCD_Template();
        selector = 0;
    }
    direction = get_button();
    if (prev_direction == NEUTRAL) {
        if (direction == LEFT || direction == RIGHT) {
            LCD_Template();
        }
        selector = side_ways_selector(selector, direction, JOKE_ARRAY_SIZE);
        splitSentencesForGLCD(jokes[selector]);
    }

    prev_direction = direction;

    if (Buttons_GetState()) return HOME_SCREEN;
    return JOKE_MACHINE;
}

int mp3_music_player() {
    LCD_Template();
    GLCD_DisplayString(6, 2, __FI, (unsigned char *)"Streaming From PC");
    stream_music();
    return HOME_SCREEN;
}

int game_center(int previous_page, char *game_center_options[]) {
    if (previous_page != GAME_CENTER) {
        LCD_Template();
        selector = 0;
    }
    direction = get_button();
    if (prev_direction == NEUTRAL) {
        selector = menu_selector(selector, direction, GAME_ARRAY_SIZE);
        screen_options_select(selector, game_center_options, GAME_ARRAY_SIZE);
    }
    prev_direction = direction;

    if (Buttons_GetState()) return HOME_SCREEN;

    if (get_button() == PRESSED) {
        // Selector's value is equal to the page's number
        return selector + 5;
    }

    return GAME_CENTER;
}

void pre_game_screen() {
    GLCD_Clear(White);
    GLCD_SetBackColor(White);
    GLCD_SetTextColor(Black);
    GLCD_DisplayString(4, 0, __FI, (unsigned char *)"- Use Joystick to move");
    GLCD_DisplayString(5, 0, __FI, (unsigned char *)"- Score is displayed");
    GLCD_DisplayString(6, 0, __FI, (unsigned char *)"  after loss");
    GLCD_DisplayString(7, 0, __FI, (unsigned char *)"- Move Joystick");
    GLCD_DisplayString(8, 0, __FI, (unsigned char *)"  to start");
    GLCD_SetTextColor(Blue);
}

int post_game_screen(int score, int game) {
    char score_str[5];
    snprintf(score_str, 5, "%d", score);
    GLCD_Clear(White);
    GLCD_SetBackColor(White);
    GLCD_SetTextColor(Black);
    GLCD_DisplayString(4, 0, __FI, (unsigned char *)"Score:");
    GLCD_DisplayString(4, 7, __FI, (unsigned char *)score_str);
    GLCD_DisplayString(5, 0, __FI, (unsigned char *)"Move Joystick:");
    GLCD_DisplayString(6, 0, __FI, (unsigned char *)"  - UP to Restart");
    GLCD_DisplayString(7, 0, __FI, (unsigned char *)"  - Down to Exit");
    GLCD_SetTextColor(Blue);
    while (1) {
        if (get_button() == UP) {
            return game;
        } else if (get_button() == DOWN) {
            return GAME_CENTER;
        }
    }
}
int snake_main(int previous_page) {
    int game_score;

    if (previous_page != SNAKE) {
        pre_game_screen();
    }
    while (!get_button());
    GLCD_Clear(White);
    GLCD_SetBackColor(White);
    game_score = snake_game();
    return post_game_screen(game_score, SNAKE);
}

int did_he_run_main(int previous_page) {
    int game_score;
    if (previous_page != DID_HE_RUN) {
        pre_game_screen();
    }
    while (!get_button());
    game_score = did_he_run_game();
    return post_game_screen(game_score, DID_HE_RUN);
}

/*
 * main: initialize and start the system
 */

int main(void) {
    int page_to_show = HOME_SCREEN;
    int new_page = HOME_SCREEN;
    int previous_page = HOME_SCREEN;

    char *home_screen_options[] = {"Photo Gallery", "MP3 Player",
                                   "Joke Machine", "Game Center"};
    char *game_center_options[] = {"Snake", "Did He Run?"};
    LED_Init();
    GLCD_Init();
    LCD_Template();
    KBD_Init();
    Buttons_Initialize();

    for (;;) {
        previous_page = page_to_show;  // Update previous_page before
                                       // page_to_show is updated
        page_to_show = new_page;
        switch (page_to_show) {
            case HOME_SCREEN:
                new_page = home_screen_main(previous_page, home_screen_options);
                break;
            case PHOTO_GALLERY:
                new_page = photo_gallery(previous_page);
                break;
            case MP3_PLAYER:
                // always returns to home screen
                new_page = mp3_music_player();
                break;
            case JOKE_MACHINE:
                new_page = joke_machine(previous_page);
                break;
            case GAME_CENTER:
                new_page = game_center(previous_page, game_center_options);
                break;
            case SNAKE:
                new_page = snake_main(previous_page);
                break;
            case DID_HE_RUN:
                new_page = did_he_run_main(previous_page);
                break;
        }
    }
}

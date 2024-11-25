#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H
#define SCREEN_LENGTH   8
#define SCREEN_WIDTH    18

// Font index for GLCD (16x24)
#define __FI            1

// Snake game variables
extern int snake[144][2];     // Snake coordinates (maximum length 144)
extern int food[2];           // Food coordinates (x, y)
extern int snake_x, snake_y;  // Snake head position
extern int snake_length;      // Length of the snake

// Function prototypes
void setup_window(void);                 // Sets up the initial game window
void init_game(void);                    // Initializes the snake game state
void generate_food(void);                // Generates food at random position
int generate_food_helper(int food_x, int food_y); // Helper function to check if food overlaps snake
int detect_collisions(void);             // Detects if snake collides with itself or walls
void update_snake(void);                 // Updates snake position and displays on the screen
int snake_game(void);                   // Main game loop, handles user input and game logic

#endif // SNAKE_GAME_H

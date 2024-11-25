#ifndef JOKES_H
#define JOKES_H

#include "GLCD.h" // Include GLCD header for display functions
#include <string.h>

// Font index for GLCD
#define __FI 1

// Maximum characters per line on GLCD
#define MAX_WIDTH 22

// Array of jokes to be displayed
extern char *jokes[];

// Function to split sentences into lines and display them on GLCD
void splitSentencesForGLCD(char *sentence);

#endif // JOKE_DISPLAY_H

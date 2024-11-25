#include "GLCD.h"
#include <string.h>

// Font index 16x24
#define __FI 1
#define MAX_WIDTH 20 // Maximum characters per line

char *jokes[] = {
    "I considered building the patio by myself. But I didn't have the stones.",
    "I've got a joke about vegetables for you... but it's a bit corny.",
    "How do you fix a broken pizza? With tomato paste.",
    "Child: Dad, make me a sandwich. Dad: Poof! You're a sandwich.",
    "Why was the shirt happy to hang around the tank top? Because it was armless",
    "Sgt.: Commissar! Commissar! The troops are revolting! Commissar: Well, you're pretty repulsive yourself.",
    "How come the stadium got hot after the game? Because all of the fans left.",
    "What do you get if you cross a turkey with a ghost? A poultry-geist!",
    "I'm tired of following my dreams. I'm just going to ask them where they are going and meet up with them later.",
    "I had a pair of racing snails. I removed their shells to make them more aerodynamic, but they became sluggish.",
    "What has ears but cannot hear? A field of corn.",
    "A woman is on trial for beating her husband to death with his guitar collection. Judge says, 'First offender?' She says, 'No, first a Gibson! Then a Fender!'",
    "Today a girl said she recognized me from vegetarian club, but I'm sure I've never met herbivore.",
    "I always wanted to look into why I procrastinate, but I keep putting it off. ",
    "I needed a password eight characters long so I picked Snow White and the Seven Dwarfs.",
    "Why does Norway have barcodes on their battleships? So when they get back to port, they can Scandinavian.",
    "The invention of the wheel was what got things rolling",
    "I was wondering why the frisbee was getting bigger, then it hit me.",
    "I knew I shouldn't steal a mixer from work, but it was a whisk I was willing to take.",
    "Conjunctivitis.com - now that's a site for sore eyes.",
    "Did you hear about the guy whose whole left side was cut off? He's all right now.",
    "Can February march? No, but April may.",
    "Why did the man put his money in the freezer? He wanted cold hard cash!",
    "I have a joke about Stack Overflow, but you would say it's a duplicate.",
    "I hate perforated lines, they're tearable.",
    "I used to work at a stationery store.  But, I didn't feel like I was going anywhere.    So, I got a job at a travel agency.  Now, I know I'll be going places.",
    "I just got fired from a florist, apparently I took too many leaves.",
    "I went to the doctor today and he told me I had type A blood but it was a type O.",
    "I decided to sell my Hoover well it was just collecting dust.",
    "What is a vampire's favorite fruit? A blood orange.",
    "It's hard to explain puns to kleptomaniacs, because they take everything literally.",
    "Did you hear about the bread factory burning down? They say the business is toast.",
    "'Why do programmers wear glasses? Because they need to C#",
    "I applied to be a doorman but didn't get the job due to lack of experience. That surprised me, I thought it was an entry level position.",
    "I bought shoes from a drug dealer once. I don't know what he laced them with, but I was tripping all day.",
    "I think circles are pointless.",
    "What do you get when you cross a rabbit with a water hose? Hare spray.",
    "What is the hardest part about sky diving? The ground.",
    "If two vegans are having an argument, is it still considered beef?",
    "What do you call a fake noodle? An impasta.",
    "Why was the robot angry? Because someone kept pressing his buttons!",
    "Why does a Moon-rock taste better than an Earth-rock? Because it's a little meteor.",
    "Q: What did the spaghetti say to the other spaghetti? A: Pasta la vista, baby!",
    "I used to work for a soft drink can crusher. It was soda pressing."
		};



// Function to split sentences into lines that fit the GLCD
void splitSentencesForGLCD(char *sentence) {
    char buffer[MAX_WIDTH + 1]; // Temporary buffer to hold a single line
		int starting_line = 3;

		int len = strlen(sentence);
		int start = 0;

		while (start < len) {
				int end = start + MAX_WIDTH;

				// Ensure we don't exceed sentence length
				if (end > len) {
						end = len;
				}

				// Find the last space within the range
				while (end > start && sentence[end] != ' ') {
						end--;
				}

				// If no space was found, break the word forcibly
				if (end == start) {
						end = start + MAX_WIDTH;
				}

				// Copy the segment into the buffer and null-terminate it
				strncpy(buffer, sentence + start, end - start);
				buffer[end - start] = '\0';

				// Print the line (replace this with the GLCD display function)
				
				GLCD_DisplayString(starting_line++, 0, __FI, (unsigned char *)buffer);


				// Move the start position forward, skipping the space
				start = end + 1;
		}
}


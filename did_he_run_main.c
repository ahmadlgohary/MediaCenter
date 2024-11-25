#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "GLCD.h"
#include "KBD.h"
#include "did_he_run_images.h"

#define UP 				8 
#define DOWN			32
#define LEFT			64
#define RIGHT			16
#define NEUTRAL   0
#define PRESSED		1

int score = 0;

void delay__(){
	int i;
	for(i = 0; i<50; i++){
		__nop();
	}
	
}
int move_player(int lane, int direction){

		
		if(direction == UP){
			if(lane == 1 || lane == 2){
				lane--;
			}

		}else if(direction == DOWN){
			if(lane == 0 || lane == 1){
				lane++;
			} 
		}
		return lane;
}
int detect_collisions_(int diddy_lane, int bottle_lane, int bottle_x, int not_police_lane, int police_x){
	// 49 is diddy far right edge
	// 29 is the bottle's image width
	if(diddy_lane == bottle_lane && (bottle_x <= 49 || (bottle_x + 29) <= 1 )){ 
		// this a collision with the bottle, so increase score by 1 and return 0
		score++;
		return 0;
	}
	else if(diddy_lane != not_police_lane && (police_x <= 49 || (police_x + 29) <= 1 )){
		// this a collision with the police, so return 1 to exit the game
		return 1;
	}
	// returning 2 means no collison
	return 2;
}
int did_he_run_game(){
		int player_position = 0;
		int police_empty_position;
		int baby_oil_position;
		int direction;
		int	prev_direction = 0;
		int i, j;
		int bottle_horizontal_position;
		int bottle_starting_horizontal_position = 150;
		int police_horizontal_position = 270;
		int collision_value = 2;
		i = 0;
		j = 0;
		score = 0;	
		police_empty_position = rand() % 3;
		baby_oil_position = rand() % 3;

		GLCD_Bitmap(0,0,320,241, (unsigned char *)diddy_run_images[0]);				// road		
		GLCD_Bitmap(1,11,48,68, (unsigned char *)diddy_run_images[1]); 				// diddy lane 1
		while(1){			
			direction = get_button();
			if(prev_direction == NEUTRAL){
				player_position = move_player(player_position, direction);			
			}
			if(prev_direction != direction){
				switch(player_position){
					case 0:
						GLCD_Bitmap(1,11,48,68, (unsigned char *)diddy_run_images[1]); 				// diddy lane 1
						GLCD_Bitmap(1,85,48,68, (unsigned char *)diddy_run_images[4]); 				// clear diddy lane 2
						GLCD_Bitmap(1,159,48,68, (unsigned char *)diddy_run_images[4]); 				// clear diddy lane 3
						break;
					case 1:
						GLCD_Bitmap(1,85,48,68, (unsigned char *)diddy_run_images[1]); 				// diddy lane 2
						GLCD_Bitmap(1,11,48,68, (unsigned char *)diddy_run_images[4]); 				// clear diddy lane 1
						GLCD_Bitmap(1,159,48,68, (unsigned char *)diddy_run_images[4]); 				// clear diddy lane 3

						break;
					case 2:
						GLCD_Bitmap(1,159,48,68, (unsigned char *)diddy_run_images[1]); 			// diddy lane 3
						GLCD_Bitmap(1,11,48,68, (unsigned char *)diddy_run_images[4]); 				// clear diddy lane 1
						GLCD_Bitmap(1,85,48,68, (unsigned char *)diddy_run_images[4]); 				// clear diddy lane 2
						break;
				}
			}
				
			bottle_horizontal_position = bottle_starting_horizontal_position  - i;
			police_horizontal_position =270 - j;	

			if(police_empty_position == 0){
				GLCD_Bitmap(police_horizontal_position,85,39,68, (unsigned char *)diddy_run_images[2]); 			// police lane 2
				GLCD_Bitmap(police_horizontal_position,159,39,68, (unsigned char *)diddy_run_images[2]); 		// police lane 3
			}else if(police_empty_position == 1){
				GLCD_Bitmap(police_horizontal_position,11,39,68, (unsigned char *)diddy_run_images[2]); 			// police lane 1
				GLCD_Bitmap(police_horizontal_position,159,39,68, (unsigned char *)diddy_run_images[2]); 		// police lane 3
			}else if(police_empty_position == 2){
				GLCD_Bitmap(police_horizontal_position,11,39,68, (unsigned char *)diddy_run_images[2]); 			// police lane 1
				GLCD_Bitmap(police_horizontal_position,85,39,68, (unsigned char *)diddy_run_images[2]); 			// police lane 2
			}
			
			if(baby_oil_position == 0){
				GLCD_Bitmap(bottle_horizontal_position,11,29,68, (unsigned char *)diddy_run_images[3]);			// baby oil lane 1
			}else if(baby_oil_position == 1){
				GLCD_Bitmap(bottle_horizontal_position,85,29,68, (unsigned char *)diddy_run_images[3]);			// baby oil lane 2				
			}else if(baby_oil_position == 2){
				GLCD_Bitmap(bottle_horizontal_position,159,29,68, (unsigned char *)diddy_run_images[3]);			// baby oil lane 3
			}
			
			if (bottle_horizontal_position == -29){
				bottle_starting_horizontal_position =250;
				i = 0;
			}
			if (police_horizontal_position == -39){
				police_horizontal_position = 270;		
				j = 0;
				police_empty_position = rand() % 3;
			}
			collision_value = detect_collisions_(player_position, baby_oil_position, bottle_horizontal_position, police_empty_position, police_horizontal_position);
			
			if(collision_value == 0){
				// delete the bottle 
				if(baby_oil_position == 0){
							GLCD_Bitmap(bottle_horizontal_position,11,48,68, (unsigned char *)diddy_run_images[4]); 				// clear bottle lane 1
				}else if(baby_oil_position == 1){
							GLCD_Bitmap(bottle_horizontal_position,85,48,68, (unsigned char *)diddy_run_images[4]); 				// clear bottle lane 2		
				}else if(baby_oil_position == 2){
							GLCD_Bitmap(bottle_horizontal_position,159,48,68, (unsigned char *)diddy_run_images[4]); 				// clear bottle lane 3
				}
				bottle_starting_horizontal_position = police_horizontal_position + 60;
				i=0;
				baby_oil_position = rand() % 3;
			}else if(collision_value == 1){
				return score;
			}

			i++;
			j++;
			prev_direction = direction;
			delay__();
	}
}

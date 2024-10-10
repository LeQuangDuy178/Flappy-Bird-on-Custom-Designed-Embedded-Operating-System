#include "../uart/uart0.h"
#include "../uart/uart1.h"
#include "mbox.h"
#include "framebf.h"
#include "flappy_bird.h"
//#include "utils.c"



void gameplay()
{

    

	// say hello
	uart_puts("\n\nGame On\n");

	// Initialize frame buffer
	framebf_init();

	/* Draw flappy bird resources images */
	void drawImage(int start_x, int start_y, const unsigned int data[], int width, int height){
		int index = 0;
		for (int y = start_y; y < start_y + height; y++){
			for (int x = start_x; x < start_x + width; x++){
				if ( data[index] != 0x00F6F6F6) { //only draw if the pixel is not black
					drawPixelARGB32(x, y, data[index]);
				}

				index++;
			}
		}
	}
	// ---------------------------------------------------------------------
	// TUBE INIT (from up to down)
	struct Tube {
		unsigned int height; // End of tube upper y
		unsigned int lower_height; // Start of tube lower y
		unsigned int width; // End of tube upper x (same for lower)
		unsigned int gap;
		unsigned int x; // Start of tube upper x (same for upper)
		unsigned int init_y; // Start of tube upper y
		unsigned int lower_end; // End of tube lower y
		unsigned int new_x;
		int velocity;
	};

	// Tube standard
	struct Tube tube;
	tube.gap = 240;
	tube.width = 50;
	tube.velocity = 6;
	tube.init_y = 0;
	tube.new_x = 1200;
	tube.lower_end = 960; // Equivalent with screen height

	// Tube object 1
	struct Tube tube1;
	tube1.height = 80;
	tube1.lower_height = tube1.height + tube.gap;
	tube1.x = 300;

	// Tube object 2
	struct Tube tube2;
	tube2.height = 300;
	tube2.lower_height = tube2.height + tube.gap;
	tube2.x = 600;

	// Tube object 3
	struct Tube tube3;
	tube3.height = 210;
	tube3.lower_height = tube3.height + tube.gap;
	tube3.x = 900;

	// Tube object 4
	struct Tube tube4;
	tube4.height = 160;
	tube4.lower_height = tube4.height + tube.gap;
	tube4.x = 1200;

	// Tube_list
	unsigned int tube_list0[] = {120, 360, 240, 60};
	unsigned int tube_list1[] = {240, 280, 280, 360};
	unsigned int tube_list2[] = {360, 120, 240, 400};
	unsigned int tube_list3[] = {340, 160, 200, 420};
	int tube_index = 0;

	//-------------------------------------------------------------------
	// BIRD INIT
	struct Bird {
		unsigned int x;
		unsigned int y;
		unsigned int width;
		unsigned int height;
		unsigned int gravity;
		int drop_velocity; // Should be increase overtime by gravity
	};

	// Bird object 1
	struct Bird bird;
	bird.x = 60;
	bird.y = 350;
	bird.height = 40;
	bird.width = 40;
	bird.gravity = 2;
	bird.drop_velocity = 4;

	//-------------------------------------------------------------------
	// SCORE INIT
	int score = 0;
	int tube_pass[] = {0, 0, 0, 0};

	//-------------------------------------------------------------------
	// GAME PROPERTIES
	int game_play = 1; // game_play = true
	int game_lose = 0; // game_lose = false -> true to exec new loop that reset game_play when press specific key 

	// While Run-time
	while(game_play) {

		//-------------------------------------------------------------------
		// Draw tube upper
		// drawTube(x1, y1, x2, y2, attr, fill)
		drawRectARGB32(tube1.x, tube.init_y, tube1.x + tube.width, tube1.height, 0x0000FF00, 1);
		drawRectARGB32(tube2.x, tube.init_y, tube2.x + tube.width, tube2.height, 0x0000FF00, 1);
		drawRectARGB32(tube3.x, tube.init_y, tube3.x + tube.width, tube3.height, 0x0000FF00, 1);
		drawRectARGB32(tube4.x, tube.init_y, tube4.x + tube.width, tube4.height, 0x0000FF00, 1);

		// Draw tube lower
		drawRectARGB32(tube1.x, tube1.lower_height, tube1.x + tube.width, tube.lower_end, 0x0000FF00, 1);
		drawRectARGB32(tube2.x, tube2.lower_height, tube2.x + tube.width, tube.lower_end, 0x0000FF00, 1);
		drawRectARGB32(tube3.x, tube3.lower_height, tube3.x + tube.width, tube.lower_end, 0x0000FF00, 1);
		drawRectARGB32(tube4.x, tube4.lower_height, tube4.x + tube.width, tube.lower_end, 0x0000FF00, 1);
		
		//-------------------------------------------------------------------
		// Bird draw
		// drawBird(x1, y1, x2, y2, attr, fill)
		drawImage(bird.x, bird.y, flappy_bird, bird.width, bird.height);

		// Delay between draw and redraw
		wait_msec(100);

		//-------------------------------------------------------------------
		// Draw black Rect at tube place to clear the previous tube
		// Should be clear tube function
		drawRectARGB32(tube1.x, tube.init_y, tube1.x + tube.width, tube1.height, 0x00000000, 1);
		drawRectARGB32(tube2.x, tube.init_y, tube2.x + tube.width, tube2.height, 0x00000000, 1);
		drawRectARGB32(tube3.x, tube.init_y, tube3.x + tube.width, tube3.height, 0x00000000, 1);
		drawRectARGB32(tube4.x, tube.init_y, tube4.x + tube.width, tube4.height, 0x00000000, 1);

		drawRectARGB32(tube1.x, tube1.lower_height, tube1.x + tube.width, tube.lower_end, 0x00000000, 1);
		drawRectARGB32(tube2.x, tube2.lower_height, tube2.x + tube.width, tube.lower_end, 0x00000000, 1);
		drawRectARGB32(tube3.x, tube3.lower_height, tube3.x + tube.width, tube.lower_end, 0x00000000, 1);
		drawRectARGB32(tube4.x, tube4.lower_height, tube4.x + tube.width, tube.lower_end, 0x00000000, 1);
		
		//-------------------------------------------------------------------
		// Clear bird previous after falling
		drawRectARGB32(bird.x, bird.y, bird.x + bird.width, bird.y + bird.height, 0x00000000, 1);
		bird.y += bird.drop_velocity; // Droping by velocity
		bird.drop_velocity += bird.gravity; // Accelerating velocitys
		// uart_dec(bird.drop_velocity);

		//-------------------------------------------------------------------
		// Tube moves
		tube1.x -= tube.velocity;
		tube2.x -= tube.velocity;
		tube3.x -= tube.velocity;
		tube4.x -= tube.velocity;

		//-------------------------------------------------------------------
		// Relocate the tube
		if (tube1.x == 0) {
			tube1.x = tube.new_x;
			tube1.height = tube_list0[tube_index];
			// uart_dec(tube_list0[tube_index]);
			tube1.lower_height = tube1.height + tube.gap;
			tube_pass[0] = 0;
		};

		if (tube2.x == 0) {
			tube2.x = tube.new_x;
			tube2.height = tube_list1[tube_index];
			// uart_dec(tube_list1[tube_index]);
			tube2.lower_height = tube2.height + tube.gap;
			tube_pass[1] = 0;   
		};

		if (tube3.x == 0) {
			tube3.x = tube.new_x;
			tube3.height = tube_list2[tube_index];
			// uart_dec(tube_list2[tube_index]);
			tube3.lower_height = tube3.height + tube.gap;
			tube_pass[2] = 0;
		};

		if (tube4.x == 0) {
			tube4.x = tube.new_x;
			tube4.height = tube_list3[tube_index];
			// uart_dec(tube_list3[tube_index]);
			tube4.lower_height = tube4.height + tube.gap;
			tube_pass[3] = 0;
			tube_index++;
			// uart_dec(tube_index);
		};

		//-------------------------------------------------------------------
		// Reset tube list
		if (tube_index == 4) {
			tube_index = 0;
		}

		//-------------------------------------------------------------------
		// Check collision
		// Coordinate (x2, y1) of bird collides with upper tube
		int tube_x [] = {tube1.x, tube2.x, tube3.x, tube4.x};
		int tube_height [] = {tube1.height, tube2.height, tube3.height, tube4.height};
		int tube_lower_height [] = {tube1.lower_height, tube2.lower_height, tube3.lower_height, tube4.lower_height};
		for (int object = 0; object < 4; object++) {

			//-------------------------------------------------------------------
			// Coordinate (x2, y1) of bird collides with upper tube
			if (((bird.x + bird.width) >= tube_x[object]) && 
				((bird.x + bird.width) <= (tube_x[object] + tube.width)) &&
				((bird.y) >= (tube.init_y)) &&
				((bird.y) <= (tube_height[object]))) 
			{
				bird.drop_velocity = 0;
				tube.velocity = 0;
				game_play = 0;
				game_lose = 1;
			}

			//-------------------------------------------------------------------
			// Coordinate (x2, y2) of bird collides with lower inv tube
			if (((bird.x + bird.width) >= tube_x[object]) && 
				((bird.x + bird.width) <= (tube_x[object] + tube.width)) &&
				((bird.y + bird.height) >= (tube_lower_height[object])) &&
				((bird.y + bird.height) <= (tube.lower_end))) 
			{
				bird.drop_velocity = 0;
				tube.velocity = 0;
				game_play = 0;
				game_lose = 1;
			}

			//-------------------------------------------------------------------
			// Coordinate (x1, y1) of bird collides with lower inv tube
			if (((bird.x) >= tube_x[object]) && 
				((bird.x) <= (tube_x[object] + tube.width)) &&
				((bird.y) >= (tube.init_y)) &&
				((bird.y) <= (tube_height[object]))) 
			{
				bird.drop_velocity = 0;
				tube.velocity = 0;
				game_play = 0;
				game_lose = 1;
			}


			//-------------------------------------------------------------------
			// Coordinate (x1, y2) of bird collides with lower inv tube
			if (((bird.x) >= tube_x[object]) && 
				((bird.x) <= (tube_x[object] + tube.width)) &&
				((bird.y + bird.height) >= (tube_lower_height[object])) &&
				((bird.y + bird.height) <= (tube.lower_end))) 
			{
				bird.drop_velocity = 0;
				tube.velocity = 0;
				game_play = 0;
				game_lose = 1;
			}

		}

		//-------------------------------------------------------------------
		// Check tube pass
		if (((tube1.x + tube.width) <= bird.x) && (tube_pass[0] == 0)) {
			score += 1;
			uart_puts("\nScore: ");
			uart_dec(score);
			tube_pass[0] = 1;
		}

		if (((tube2.x + tube.width) <= bird.x) && (tube_pass[1] == 0)) {
			score += 1;
			uart_puts("\nScore: ");
			uart_dec(score);
			tube_pass[1] = 1;
		}

		if (((tube3.x + tube.width) <= bird.x) && (tube_pass[2] == 0)) {
			score += 1;
			uart_puts("\nScore: ");
			uart_dec(score);
			tube_pass[2] = 1;
		}

		if (((tube4.x + tube.width) <= bird.x) && (tube_pass[3] == 0)) {
			score += 1;
			uart_puts("\nScore: ");
			uart_dec(score);
			tube_pass[3] = 1;
		}

		//-------------------------------------------------------------------
		// Get user input for bird up 
		char a = getUart();
		if (a !='a'){ // If 'a' is not entered
			//uart_puts("\nNo input");
		}
		else { // If 'a' is pressed
			uart_puts("\nJump: ");
			uart_sendc(a);
			bird.drop_velocity = 0;
			bird.drop_velocity -= 10;
		}

		//-------------------------------------------------------------------
		// Game next stage
		if (score >= 12) { // Actually only change gap in tube 16 so on
			tube.gap = 180;
			tube.velocity = 8;
		}

		//-------------------------------------------------------------------
		// Game lose exec
		while (game_lose) {
		
			// Print game_lose flag and ask for replay
			uart_puts("\nYou lose the game");
			uart_puts("\nPlay again, press d: ");

			// Get input for key pressed to play again
			char d = uart_getc();
			if (d !='d'){ // If 'd' is not entered
				//uart_puts("\nNo input");
			}
			else { // If 'd' is pressed then replay the game
				uart_puts("\nPlay again after pressing ");
				uart_sendc(d);
				game_play = 1; // Reset the game
				game_lose = 0; 
				score = 0;
				bird.y = 350;
				tube1.x = 300;
				tube2.x = 600;
				tube3.x = 900;
				tube4.x = 1200;
				tube.velocity = 6;
				tube.gap = 240;
			}

		}

	}

}
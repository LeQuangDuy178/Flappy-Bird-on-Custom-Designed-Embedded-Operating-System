#include "../uart/uart0.h"
#include "../uart/uart1.h"
#include "mbox.h"
#include "framebf.h"
#include "image1.h"
#include "image2.h"

void video_shown()
{
   

	// first word for the function 
	uart_puts("\n\nVideo Start!\n");

	// Initialize frame buffer
	framebf_init();

    // Draw Frame function 
    void drawImage(int start_x, int start_y, const unsigned long data[], int width, int height){
	    int index = 0;
	    for (int y = start_y; y < start_y + height; y++){
		    for (int x = start_x; x < start_x + width; x++){
			    if ( data[index] != 0x00FFFFFF) { //only draw if the pixel is not white (no full white in pixel data)
		    		drawPixelARGB32(x, y, data[index]);
		    	}
	
		        index++;
		    }
	    }
    }
	/* Question 2B */
	// Display video by frame increment
	const unsigned long *image_frames[10] = {myImage1, myImage2, myImage3, myImage4, myImage5, 
											myImage6, myImage7, myImage8, myImage9, myImage10		
											};
	for (int frame = 0; frame < 10; frame++){

		// Draw frame
		drawImage(0, 0, image_frames[frame], 1280, 720);

		// Delay
		wait_msec(33); // 0.033 seconds for 1 frame

	}

		//adding some delay
		int r = 1000; 
		while(r--) { asm volatile("nop"); } 
		//clear 
	
}
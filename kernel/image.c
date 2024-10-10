#include "../uart/uart0.h"
#include "../uart/uart1.h"
#include "mbox.h"
#include "framebf.h"
#include "image.h"
#include "font.h"

void image_shown()
{
	// first word for the function 
	uart_puts("\n\nImage Showing\n");

	// Initialize frame buffer
	framebf_init();
	
	//insert the background image
	void drawImage(int start_x, int start_y, const unsigned int data[], int width, int height){
		int index = 0;
		for(int y = start_y; y < start_y + height; y++){
			for(int x = start_x; x < start_x + width; x++){
				if (myImage[index] != 0x00FFFFFF){
					drawPixelARGB32(x,y,myImage[index]);
					
				}
				index++;
			}
		}
	}
	drawImage(0,0,myImage,1280,960);


}
void Drawing_shown() {
void drawChar(unsigned char ch, int x, int y, unsigned int attr, int zoom)
{
    unsigned char *glyph = (unsigned char *)&font + (ch < FONT_NUMGLYPHS ? ch : 0) * FONT_BPG;

    for (int i = 1; i <= (FONT_HEIGHT*zoom); i++) {
		for (int j = 0; j< (FONT_WIDTH*zoom); j++) {
			unsigned char mask = 1 << (j/zoom);
            if (*glyph & mask) { //only draw pixels belong to the character glyph
			    drawPixelARGB32(x + j, y + i, attr);
            }
		}
		glyph += (i % zoom) ? 0 : FONT_BPL;
    }
}


void drawString(int x, int y, char *str, unsigned int attr, int zoom)
{
    while (*str) {
        if (*str == '\r') {
            x = 0;
        } else if (*str == '\n') {
            x = 0; 
			y += (FONT_HEIGHT*zoom);
        } else {
            drawChar(*str, x, y, attr, zoom);
            x += (FONT_WIDTH*zoom);
        }
        str++;
    }
}

	drawString(0,20,"Nguyen Quoc Thinh",0x0000BB00, 1);
	drawString(256,20,"Nguyen Hoang Phu",0x0000BB00, 1);
	drawString(512,20,"Le Trung Hieu",0x0000BB00, 1);
	drawString(768,20,"Le Quang Duy",0x0000BB00, 1);

	uart_puts("\nMyOs> ");
}
void clearScreen(){
	drawRectARGB32(0,0,1280,960,0x00000000,1);
}

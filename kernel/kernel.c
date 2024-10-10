#include "../uart/uart0.h"
#include "../uart/uart1.h"
#include "mbox.h"
#define MAX_history 1000
#define MAX_CMD_SIZE 500
#define KEY_SIZE 20

char history[MAX_history][MAX_CMD_SIZE];
int count = 0 ; 
int his_index = -1; 
void store_command(const char *command) {
    if ( count < MAX_history) {
        my_strncpy(history[count], command, MAX_CMD_SIZE);
        history[count][MAX_CMD_SIZE - 1] = '\0';  // Ensure null termination
        count++;
    } else {
        // Shift commands if history is full (without using free)
        for (int i = 1; i < MAX_CMD_SIZE; i++) {
            my_strncpy(history[i - 1], history[i], MAX_history);
        }
        my_strncpy(history[MAX_CMD_SIZE - 1], command, MAX_history);
        history[MAX_CMD_SIZE - 1][MAX_history - 1] = '\0';
    }
    his_index = count;  // Reset history index after new command
}
int integer_extract(const char* str){
	int number = 0;
    int sign = 1;

    for (int i = 0; str[i] != '\0'; i++) {
        // Check for negative sign
        if (str[i] == '-') {
            sign = -1;
        }

        // Check if the character is between '0' and '9' (ASCII 48 to 57)
        if (str[i] >= '0' && str[i] <= '9') {
            number = number * 10 + (str[i] - '0');
        }
    }

    return sign * number;
}
void my_strncpy(char *dest, const char *src, int max_size) {
    int i = 0;
    // Copy each character from src to dest, ensuring we don't exceed max_size - 1
    while (src[i] != '\0' && i < max_size - 1) {
        dest[i] = src[i];
        i++;
    }
    // Add the null terminator at the end of the destination string
    dest[i] = '\0';
}
// compare the value of the character in the array
int my_strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}
// function to check for the TAB button 
int my_strncmp(const char *str1, const char *str2, unsigned int n) {
    // Iterate through both strings for up to n characters
    for (unsigned int i = 0; i < n; i++) {
        // If characters differ or one string ends
        if (str1[i] != str2[i] || str1[i] == '\0' || str2[i] == '\0') {
            return (unsigned char)str1[i] - (unsigned char)str2[i];
        }
    }
    return 0;  // Strings are equal up to n characters
}
void my_strcpy(char *dest, const char *src) {
    char *ptr = dest; // Keep a pointer to the start of the destination string

    while (*src != '\0') { // Loop until the end of the source string
        *dest = *src;      // Copy the character from source to destination
        dest++;           // Move to the next position in the destination string
        src++;            // Move to the next position in the source string
    }

    *dest = '\0'; // Null-terminate the destination string

    return ptr; // Return the original pointer to the destination string
}
int my_strlen(const char *str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}
unsigned int sizeof_char() {
    char a;
    return (char*)(&a + 1) - (char*)&a;
}
void navigate_up(char *input) {
    if (his_index > 0) {
        his_index--;
        my_strncpy(input, history[his_index], MAX_history);
        input[MAX_history - 1] = '\0';  // Ensure null termination
        uart_puts(input); // Print the command
       
    }
}

// Function to handle DOWN (plus) key
void navigate_down(char *input) {
    if (his_index < count - 1) {
        his_index++;
        my_strncpy(input, history[his_index], MAX_history);
        input[MAX_history - 1] = '\0';  // Ensure null termination
    } else {
        his_index = count;  // To reset after the last command
        input[0] = '\0';  // Clear input if at the bottom
    }
    uart_puts(input); // Print the command
  
}

void cli()
{
	
	static char cli_buffer[MAX_CMD_SIZE];
	static int index = 0;
	

	//read each char
	char c = uart_getc();
	
	int prompt_length = 5;
	//put into a buffer until got new line character
	if (c != '\n'){
		cli_buffer[index] = c; //Store into the buffer
		index++;
		// for Backspace
		
		if(c == '\b' || c == 127 || c == 8){
			uart_puts("\b \b");}
			else {
				uart_sendc(c);
			}
			
	// for TAB 
	if(c == '\t'|| c == 9){
		if (my_strncmp(cli_buffer,"help",1)  == 0 ){
			uart_puts("\b\bhelp");
		}
		if (my_strncmp(cli_buffer,"showInfo",1)== 0){
			uart_puts("\b\bshowInfo");
		}
		if (my_strncmp(cli_buffer,"baudrate",1)== 0){
			uart_puts("\b\bbaudrate");
		}
		if (my_strncmp(cli_buffer,"image",1)== 0){
			uart_puts("\b\bimage");
		}
		if (my_strncmp(cli_buffer,"clear",1)== 0){
			uart_puts("\b\bclear");
		}
		if (my_strncmp(cli_buffer,"Stopbit",1)== 0){
			uart_puts("\b\bStopbit");
		}
		if (my_strncmp(cli_buffer,"draw",1)== 0){
			uart_puts("\b\bdraw");
		}
		if (my_strncmp(cli_buffer,"game",1)== 0){
			uart_puts("\b\bgame");
		}
		if (my_strncmp(cli_buffer,"video",1)== 0){
			uart_puts("\b\bvideo");
		}
		
	}
	else if ( c == '_'){
		navigate_up(cli_buffer);
		index = my_strlen(cli_buffer);
	}
	else if (c == '+'){
		navigate_down(cli_buffer);
		index = my_strlen(cli_buffer);
	}
	}
	// if enter is pressed 
	else if (c == '\n'){
		if (index > 0){
			store_command(cli_buffer);
		}
		cli_buffer[index] = '\0';
		
    
    // Wrap the history index if it exceeds the max
    
    
		
		// history_strorage(history,10,cli_buffer);
		// navigation(cli_buffer,history);
		if(my_strcmp(cli_buffer,"help")== 0 ){
			uart_puts("\nBrief information for all commands\n help <command name>: show all information about the specific command \nclear: clear the screen which scroll down all to the current position of the cursor \nshowInfo: show all of the information of the board and the MAC address\nbaudrate: set the current UART baudrate which have the value defined on the Teraterm\nStopbit: set the stopbit of the UART and the value will be 1 or 2\nMyOs> ");
			store_command("\nBrief information for all commands\n help <command name>: show all information about the specific command \nclear: clear the screen which scroll down all to the current position of the cursor \nshowInfo: show all of the information of the board and the MAC address\nbaudrate: set the current UART baudrate which have the value defined on the Teraterm\nStopbit: set the stopbit of the UART and the value will be 1 or 2\nMyOs> ");
		}
		if(my_strcmp(cli_buffer,"h\t")== 0 ){
			uart_puts("\nBrief information for all commands\n help <command name>: show all information about the specific command \nclear: clear the screen which scroll down all to the current position of the cursor \nshowInfo: show all of the information of the board and the MAC address\nbaudrate: set the current UART baudrate which have the value defined on the Teraterm\nStopbit: set the stopbit of the UART and the value will be 1 or 2\nMyOs> ");
			store_command("\nBrief information for all commands\n help <command name>: show all information about the specific command \nclear: clear the screen which scroll down all to the current position of the cursor \nshowInfo: show all of the information of the board and the MAC address\nbaudrate: set the current UART baudrate which have the value defined on the Teraterm\nStopbit: set the stopbit of the UART and the value will be 1 or 2\nMyOs> ");
		}
		
		else if(my_strcmp(cli_buffer,"help <showInfo>")== 0 ){
			uart_puts("\nthis is the command to show all of the information of the Board Information in hexadecimal numbers and the MAC address in the format of: XX:XX:XX:XX:XX:XX\nMyOs> ");
			store_command("\nthis is the command to show all of the information of the Board Information in hexadecimal numbers and the MAC address in the format of: XX:XX:XX:XX:XX:XX");
		}
		else if(my_strcmp(cli_buffer,"h\t <showInfo>")== 0 ){
			uart_puts("\nthis is the command to show all of the information of the Board Information in hexadecimal numbers and the MAC address in the format of: XX:XX:XX:XX:XX:XX\nMyOs> ");
			store_command("\nthis is the command to show all of the information of the Board Information in hexadecimal numbers and the MAC address in the format of: XX:XX:XX:XX:XX:XX\n");
		}
		
		else if(my_strcmp(cli_buffer,"help <baudrate>")== 0){
			uart_puts("\nthis is the command for the user to change the baudrate of the current UART \n To change the value, just type: baudrate <number>\\nMyOs> ");
			store_command("\nthis is the command for the user to change the baudrate of the current UART \n To change the value, just type: baudrate <number>");
		}
		else if(my_strcmp(cli_buffer,"h\t <baudrate>")== 0){
			uart_puts("\nthis is the command for the user to change the baudrate of the current UART \n To change the value, just type: baudrate <number>\nMyOs> ");
			store_command("\nthis is the command for the user to change the baudrate of the current UART \n To change the value, just type: baudrate <number>");
		}
		
		else if(my_strcmp(cli_buffer,"help <Stopbit>")== 0){
			uart_puts("\nthis is the command that allow the user changing stopbit of the UART currently used\nTo change the stopbit, just type: stopbit 1 or stopbit 2\nMyOs> ");
			store_command("\nthis is the command that allow the user changing stopbit of the UART currently used\nTo change the stopbit, just type: stopbit 1 or stopbit 2");
		}
		else if(my_strcmp(cli_buffer,"h\t <Stopbit>")== 0){
			uart_puts("\nthis is the command that allow the user changing stopbit of the UART currently used\nTo change the stopbit, just type: stopbit 1 or stopbit 2\nMyOs> ");
			store_command("\nthis is the command that allow the user changing stopbit of the UART currently used\nTo change the stopbit, just type: stopbit 1 or stopbit 2");
		}
		// show information of the board 
		else if(my_strcmp(cli_buffer,"showInfo")== 0){
			Inforreading();
			store_command("Buffer Adrress: 0x04A4E000\nGot successful response\nResponse code for the whole message: 0x80000000\n+Response Code in Message Tag: 0x80000004\nData: Board revision value = 0x00B03115\n+Response Code in Message TAG: 0x00010003\nDATA: Board MAC address = 0x80000000652:54:00:12:34:57");
		}
		else if(my_strcmp(cli_buffer,"s\t")== 0){
			Inforreading();
			store_command("Buffer Adrress: 0x04A4E000\nGot successful response\nResponse code for the whole message: 0x80000000\n+Response Code in Message Tag: 0x80000004\nData: Board revision value = 0x00B03115\n+Response Code in Message TAG: 0x00010003\nDATA: Board MAC address = 0x80000000652:54:00:12:34:57");
		}
		// clear 
		else if(my_strcmp(cli_buffer,"clear")== 0){
			uart_puts("\033[2J\033[H MyOs> ");
		}
		else if(my_strcmp(cli_buffer,"c\t")== 0){
			uart_puts("\033[2J\033[H MyOs> ");
		}
		// image 
		else if (my_strcmp(cli_buffer,"image")== 0){
			image_shown();
			store_command("Image Showing\n\nBuffer Address: 0x04A4E130\nGot successful response\nGot allocated Frame Buffer at RAM physical address: 0x3C100000\nFrame Buffer Size (bytes): 3145728");
		}
		else if (my_strcmp(cli_buffer,"i\t")== 0){
			image_shown();
			store_command("Image Showing\n\nBuffer Address: 0x04A4E130\nGot successful response\nGot allocated Frame Buffer at RAM physical address: 0x3C100000\nFrame Buffer Size (bytes): 3145728");
		}
		// baudrate setting 
		else if (my_strncmp(cli_buffer,"baudrate ",9)== 0){
			int baud = integer_extract(cli_buffer);
			uart_init(baud,1);
			uart_puts("\nuart baudrate set succsessful, baudrate is: ");
			uart_dec(baud);
			store_command("\nuart baudrate set succsessful, baudrate is:");
		}
		else if (my_strncmp(cli_buffer,"b\t ",3)== 0){
			int baud = integer_extract(cli_buffer);
			uart_init(baud,1);
			uart_puts("\nuart baudrate set succsessful, baudrate is: ");
			uart_dec(baud);
			store_command("\nuart baudrate set succsessful, baudrate is:");
		}
		// stopbit 
		else if(my_strncmp(cli_buffer,"Stopbit ",8)== 0){
			int stop = integer_extract(cli_buffer);
			uart_puts("\nuart stopbit set successful, stopbit is: ");
			uart_dec(stop);
			if (stop == 1){
				store_command("\nuart stopbit set successful, stopbit is: 1");
			} else if (stop == 2){
				store_command("\nuart stopbit set successful, stopbit is: 2");
			}
		}
		else if(my_strncmp(cli_buffer,"S\t ",3)== 0){
			int stop = integer_extract(cli_buffer);
			
			uart_puts("\nuart stopbit set successful, stopbit is: ");
			uart_dec(stop);
			
			if (stop == 1){
				store_command("\nuart stopbit set successful, stopbit is: 1");
			} else if (stop == 2){
				store_command("\nuart stopbit set successful, stopbit is: 2");
			}
		}
		// drawing function 
		else if(my_strcmp(cli_buffer,"draw") == 0){
		 	Drawing_shown();
		 }
		else if(my_strcmp(cli_buffer,"d\t") == 0){
		 	Drawing_shown();
		}
		//clear the screen
		else if (my_strcmp(cli_buffer,"clear screen")== 0){
			clearScreen();
		}
		// game session 
		else if(my_strcmp(cli_buffer,"game")== 0){
			gameplay();
			store_command("\n\nGame On\n");
		}
		else if (my_strcmp(cli_buffer,"g\t")== 0){
			gameplay();
			store_command("\n\nGame On\n");    
		}
		// showing the video 
		else if(my_strcmp(cli_buffer,"video")== 0){
			video_shown();
			store_command("\n\nVideo Start!\n");
		}
		else if(my_strcmp(cli_buffer,"v\t")== 0){
			video_shown();
			store_command("\n\nVideo Start!\n");
		}
		else if (my_strcmp(cli_buffer,"\0") == 0){
			uart_puts("\nMyOs> ");
		}
		else{
			uart_puts("\nwrong command, Please refer to the available command provided\nType help for more information\nMyOs> ");
			store_command("\nwrong command, Please refer to the available command provided\nType help for more information\nMyOs> ");
		}
		
		
		
		//Return to command line
		index = 0;
	} 
}

		




void main(){
    // set up serial console
	uart_init(115200);
	framebf_init();
	print_welcome_message();
	uart_puts("MyOs> ");
	
    // run CLI
    while(1) {
    	cli();
	}
}

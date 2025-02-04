//#include "font.h"
#include "framebf.h"
#include "../uart/uart0.h"
#include "../uart/uart1.h"


#if 0//UART0
unsigned int uart_isReadByteReady(){
	return ( !(UART0_FR & UART0_FR_RXFE) );
}

#else //UART1
unsigned int uart_isReadByteReady(){
	return (AUX_MU_LSR & 0x01);
}
#endif

unsigned char getUart(){
    unsigned char ch = 0;
    if (uart_isReadByteReady())
    	ch = uart_getc();
    return ch;
}




/* Functions to delay, set/wait timer */

void wait_msec(unsigned int msVal){
    register unsigned long f, t, r, expiredTime; //64 bits

    // Get the current counter frequency (Hz), 1Hz = 1 pulses/second
    asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));
    
    // Read the current counter value
    asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
    
    // Calculate expire value for counter
    /* Note: both expiredTime and counter value t are 64 bits,
    thus, it will still be correct when the counter is overflow */  
    expiredTime = t + f * msVal / 1000;

    do {
    	asm volatile ("mrs %0, cntpct_el0" : "=r"(r));
    } while(r < expiredTime);
}


void set_wait_timer(int set, unsigned int msVal) {
    static unsigned long expiredTime = 0; //declare static to keep value
    register unsigned long r, f, t;
    
    if (set) { /* SET TIMER */
        // Get the current counter frequency (Hz)
        asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));

        // Read the current counter
        asm volatile ("mrs %0, cntpct_el0" : "=r"(t));

        // Calculate expired time:
        expiredTime = t + f * msVal / 1000;
    } 
    else { /* WAIT FOR TIMER TO EXPIRE */
        do {
            asm volatile ("mrs %0, cntpct_el0" : "=r"(r));
        } while(r < expiredTime);
    }
}
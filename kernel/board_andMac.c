#include "mbox.h"
#include "../uart/uart0.h"
#include "../uart/uart1.h"

void Inforreading()
{
   
    // say hello
   //uart_puts("Hello World!\n");
    
    // mailbox data buffer: Read ARM frequency
    mBuf[0] = 12*4; // Message Buffer Size in bytes (17 elements * 4 bytes (32 bit) each)
    mBuf[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
    
    mBuf[2] = 0x00010002; // TAG Identifier: Get board revision
    mBuf[3] = 4; // Value buffer size in bytes (max of request and response lengths)
    mBuf[4] = 0; // REQUEST CODE = 0
    mBuf[5] = 0; // clear output buffer (response data are mBuf[10])

    mBuf[6] = 0x00010003; // TAG Identifier: Get board MAC address
    mBuf[7] = 8; // Value buffer size in bytes (require 6 but we need to use two elements)
    mBuf[8] = 0; // REQUEST CODE = 0
    mBuf[9] = 0; // clear output buffer (MAC address in network byte order)
    mBuf[10] = 0; // clear output buffer (response data are mBuf[9] & mBuf[10])

    mBuf[11] = MBOX_TAG_LAST;
    
    //Note: Board model and Board serial may give 0 values on QEMU. 
    //Board revision, board MAC address: try it yourself, since it is assignment tasks.

    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP)) {
        uart_puts("\nResponse Code for whole message: ");
        uart_hex(mBuf[1]);


        uart_puts("\n\n+ Response Code in Message TAG: ");
        uart_hex(mBuf[4]);
        uart_puts("\nDATA: Board revision value = ");
        uart_hex(mBuf[5]);

        uart_puts("\n\n+ Response Code in Message TAG: ");
        uart_hex(mBuf[6]);
        uart_puts("\nDATA: Board MAC address = ");
        uart_hex(mBuf[8]);
        
        unsigned char *byte_ptr = (unsigned char *) (&mBuf[9]);
        //Read six bytes from the lowest byte of mBuf[14]
        //(4 bytes of mBuf[14] and 2 bytes of mBuf[15])
        for (int i = 0; i <= 5; i++){
            uart_hex_no_base(*byte_ptr, 2); //print out two hexa digits from the byte value
            if (i != 5){
                uart_puts(":");
            }
            byte_ptr++; //access next byte
        }
        uart_puts("\nMyOs> ");
    } else {
        uart_puts("Unable to query!\n");
    }
}
#include "mbox.h"
#include "../uart/uart1.h"


void print_welcome_message() {
    uart_puts("oooooooooooo oooooooooooo oooooooooooo ooooooooooooo   .oooo.         .o    .ooooo.     .oooo.    \n");
    uart_puts("`888'     `8 `888'     `8 `888'     `8 8'   888   `8 .dP  888b      .d88   888' `Y88.  d8P'`Y8b   \n");
    uart_puts(" 888          888          888              888             ]8P'  .d'888   888    888 888    888  \n");
    uart_puts(" 8880008      888oooo8     888ooo8          888          .d8P'  .d'  888    `Vbood888 888    888  \n");
    uart_puts(" 888          888          888              888        .dP'     88ooo888oo       888' 888    888  \n");
    uart_puts(" 888       o  888       o  888       o      888      .oP     .o      888       .88P'  `88b  d88'  \n");
    uart_puts("o888ooooood8 o888ooooood8 o888ooooood8     o888o     8888888888     o888o    .oP'      `88bd8P'   \n");
    uart_puts("\n");
    uart_puts("oooooooooo.        .o.       ooooooooo.   oooooooooooo         .oooooo.    .oooooo..o        \n");
    uart_puts("`888'   `Y8b      .888.      `888   `Y88. `888'     `8        d8P'  `Y8b  d8P'    `Y8        \n");
    uart_puts(" 888     888     .8 888.      888   .d88'  888               888      888 Y88bo.             \n");
    uart_puts(" 888oooo888'    .8' `888.     888ooo88P'   888oooo8          888      888   `Y8888o.         \n");
    uart_puts(" 888    `888   .88ooo8888.    888`88b.     888               888      888      ` Y88b        \n");
    uart_puts(" 888    .88P  .8'     `888.   888  `88b.   888       o       `88b    d88' oo     .d8P        \n");
    uart_puts("o888bood8P'  o88o     o8888o o888o  o888o o888ooooood8        `Y8bood8P'  888888P'           \n");
    uart_puts("\n");
    uart_puts("Developed by <NGUYEN QUOC THINH> - <S3924426> \n");
    uart_puts("Developed by <NGUYEN HOANG PHU> - <S3847197> \n");
    uart_puts("Developed by <LE TRUNG HIEU> - <S3877456> \n");
    uart_puts("Developed by <LE QUANG DUY> - <S3880200> \n");
}

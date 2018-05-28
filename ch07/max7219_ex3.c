#include <stdio.h>
#include <pigpiod_if2.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <termios.h>
#include "max7219_functions.h"

int main(void)
{
    static struct termios oldtio, newtio;
    tcgetattr(0, &oldtio);
    newtio = oldtio;
    newtio.c_lflag &= ~ICANON;
    newtio.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &newtio);

    init_max7219();

    int c;
    int row = 1;
    int col = 1;
    dot_max7219(row, col);
    while((c = getc(stdin)) != EOF){
        switch(c){
            case 27:
                c = getc(stdin);
                if(c == 91){
                    c = getc(stdin);
                    if(c == 68) col--;
                    else if(c == 67) col++;
                    else if(c == 65) row--;
                    else if(c == 66) row++;
                }
                break;
        }
        if(col > 8) col = 8;
        else if(col < 1) col = 1;
        else if(row > 8) row = 8;
        else if(row < 1) row = 1;
        printf("[%d,%d]\r", row, col);
        dot_max7219(row, col);
    }
    if(ferror(stdin)){
        printf("input error\n");        return 1;
    }

    tcsetattr(0, TCSANOW, &oldtio);

    release_max7219();
    return 0;
}


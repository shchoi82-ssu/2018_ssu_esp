#include<stdio.h>
#include<unistd.h>
#include<termios.h>
#define BUFFSIZE    4096

int main(void)
{
    static struct termios oldtio, newtio;

    tcgetattr(0, &oldtio);
    newtio = oldtio;
    newtio.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &newtio);

    /*******************************/
    int c;
    while((c = getc(stdin)) != EOF)
        if(putc(c, stdout) == EOF){
            printf("output error\n");
            return 1;
        }
    if(ferror(stdin)){
        printf("input error\n");
        return 1;
    }
    /*******************************/

    tcsetattr(0, TCSANOW, &oldtio);
    return 0;
}




#include<stdio.h>
#include<unistd.h>
#include<termios.h>

int main(void)
{
    static struct termios oldtio, newtio;

    tcgetattr(0, &oldtio);
    newtio = oldtio;
    newtio.c_lflag &= ~ICANON;
    newtio.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &newtio);

    /*******************************/
    int c;
    while((c = getc(stdin)) != EOF){
        switch(c){
            case 27:    
                c = getc(stdin);
                if(c == 91){
                    c = getc(stdin);
                    if(c == 68) printf("<LEFT>\n");
                    else if(c == 67) printf("<RIGHT>\n");
                    else if(c == 65) printf("<UP>\n");
                    else if(c == 66) printf("<DOWN>\n");
                }
                break;
            case 32:
                printf("<SPACE BAR>\n");
                break;
            case 10:
                printf("<ENTER>\n");
                break;
        }
    }
    if(ferror(stdin)){
        printf("input error\n");
        return 1;
    }
    /*******************************/

    tcsetattr(0, TCSANOW, &oldtio);
    return 0;
}




#include <stdio.h>
int main(void)
{
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
    return 0;
}


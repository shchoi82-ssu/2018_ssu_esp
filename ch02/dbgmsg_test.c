#include <stdio.h>
#include <unistd.h>
#include "dbgmsg.h"

void f(void);

int main(void)
{
    int i;
    for(i = 1 ; i <= 20 ; i++) {
        DBGMSG("%d job processing", i);
        f();
    }
    printf("completed\n");
    return 0;
}

void f(void)
{
    sleep(1);
}


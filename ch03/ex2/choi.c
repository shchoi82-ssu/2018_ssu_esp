#include<stdio.h>

int i = 10;

void choi_f1()
{
    printf("[%s] in %s:%d\n", __FILE__, __func__, __LINE__);
}

void choi_f2()
{
    printf("[%s] in %s:%d\n", __FILE__, __func__, __LINE__);
}



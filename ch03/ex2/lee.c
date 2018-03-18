#include<stdio.h>

static void lee_f2();

void lee_f1()
{
    printf("[%s] in %s:%d\n", __FILE__, __func__, __LINE__);
    lee_f2();
}

static void lee_f2()
{
    printf("[%s] in %s:%d\n", __FILE__, __func__, __LINE__);
}



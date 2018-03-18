#include<stdio.h>

void choi_f1();
void choi_f2();
void kim_f1();
void kim_f2();
void lee_f1();

extern int i;

int main(void)
{
    choi_f1();
    choi_f2();
    kim_f1();
    kim_f2();
    lee_f1();

    printf("[%s] in %s:%d\n", __FILE__, __func__, __LINE__);
    printf("i = %d\n", i);
    return 0;
}



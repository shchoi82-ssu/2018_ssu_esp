#include <stdio.h>

#ifndef PI
#define PI 3.14
#endif

int main(void)
{
    int radius;
    printf("radius:");
    scanf("%d", &radius);
#ifdef DEBUG
    printf("address of radius:%p\n", &radius);
#endif
    printf("Circumference:%f\n", radius * PI);
    return 0;
}


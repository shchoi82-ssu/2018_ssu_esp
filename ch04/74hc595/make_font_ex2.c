#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "8x8font.h"

int main(void)
{
    int i, j, k;
    uint8_t mask;

    for(k = 0 ; k < FONT88_LEN ; k++){
        for(i = 0 ; i < FONT_ROW ; i++){      // row
            for(j = 0 ; j < FONT_COL ; j++){  // col
                mask = 0b1 << (7-j);
                printf("%s", (mask & FONT88[k][i]) ? "o ":". ");
            }
            printf("\n");
        }
        usleep(500000);
        printf("\n");
    }

    return 0;
}



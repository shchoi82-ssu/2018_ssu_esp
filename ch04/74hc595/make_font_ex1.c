#include <stdio.h>
#include <stdint.h>

#define FONT_ROW 8
#define FONT_COL 8

// letter 'A'
uint8_t font88[FONT_COL] = { 
                        0b00000000,
                        0b00111100,
                        0b01100110,
                        0b01100110,
                        0b01111110,
                        0b01100110,
                        0b01100110,
                        0b01100110 };

int main(void)
{
    int i, j;
    uint8_t mask;

    for(i = 0 ; i < FONT_ROW ; i++){      // row
        for(j = 0 ; j < FONT_COL ; j++){  // col
        	mask = 0b1 << (7-j);
			printf("%s", (mask & font88[i]) ? "o ":". ");
        }
        printf("\n");
    }

    return 0;
}



#define MAX7219_REG_DECODEMODE  0x9
#define MAX7219_REG_INTENSITY   0xA
#define MAX7219_REG_SCANLIMIT   0xB
#define MAX7219_REG_SHUTDOWN    0xC
#define MAX7219_REG_DISPLAYTEST 0xF
#define SPI_CHANNEL             0
#define SPI_CLOCK               10e6

int init_max7219();
void release_max7219();

void spi_write_max7219(uint8_t reg, uint8_t val);
void clear_max7219();
void dot_max7219(int row, int col);

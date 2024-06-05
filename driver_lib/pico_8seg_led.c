#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "device_drivers.h"
#include "hardware_config.h"

#define SPI_PORT spi0
#define PIN_CS 17
#define PIN_SCK 18
#define PIN_MOSI 19

#define CMD_DISPLAY_CONTROL 0x88 // Example command, consult datasheet for exact values
#define CMD_DISPLAY_ADDRESS 0xC0 // Example command to set address

// Initialize the SPI interface and the 8-segment LED display
int pico_8seg_led_init(void) {
    // Initialize SPI port
    spi_init(SPI_PORT, 500 * 1000); // 500 kHz
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    
    // Initialize CS pin
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
    return 1;
}

// Send a command to the 8-segment LED display
void pico_8seg_led_send_command(uint8_t command) {
    gpio_put(PIN_CS, 0);
    spi_write_blocking(SPI_PORT, &command, 1);
    gpio_put(PIN_CS, 1);
}

// Send data to the 8-segment LED display
void pico_8seg_led_send_data(uint8_t address, uint8_t data) {
    uint8_t buf[2] = {address, data};
    gpio_put(PIN_CS, 0);
    spi_write_blocking(SPI_PORT, buf, 2);
    gpio_put(PIN_CS, 1);
}


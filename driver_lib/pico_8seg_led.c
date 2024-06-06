#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "device_drivers.h"
#include "hardware_config.h"
#include "utils.h"

#define SPI_PORT spi1
int LED_8SEG_RCLK_PIN;
int LED_8SEG_CLK_PIN;
int LED_8SEG_MOSI_PIN;

// Initialize the SPI interface and the 8-segment LED display
int pico_8seg_led_init(void) {
    LED_8SEG_RCLK_PIN      = 9;
    LED_8SEG_CLK_PIN     = 10;
    LED_8SEG_MOSI_PIN    = 11;
    
    // SPI Config
    spi_init(SPI_PORT, 10000 * 1000);
    gpio_set_function(LED_8SEG_CLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(LED_8SEG_MOSI_PIN, GPIO_FUNC_SPI);
    // GPIO Config
    GPIO_MODE(LED_8SEG_RCLK_PIN, 1);
    gpio_put(LED_8SEG_RCLK_PIN, 1);
    if (spi_get_hw(SPI_PORT) == NULL) {
        return 0;
    }
    pico_8seg_led_send_command(0X57, 0x35);
    return 1;
}

void pico_8seg_led_send_command(UBYTE const Num, UBYTE const Seg) {
    gpio_put(LED_8SEG_RCLK_PIN, 1);
    spi_write_blocking(SPI_PORT, &Num, 1);
    spi_write_blocking(SPI_PORT, &Seg, 1);
    gpio_put(LED_8SEG_RCLK_PIN, 0);
    sleep_ms(2);
    gpio_put(LED_8SEG_RCLK_PIN, 1);
}

void GPIO_MODE(UWORD Pin, UWORD Mode) {
    gpio_init(Pin);
    if(Mode == 0 || Mode == GPIO_IN) {
        gpio_set_dir(Pin, GPIO_IN);
    } else {
        gpio_set_dir(Pin, GPIO_OUT);
    }
}
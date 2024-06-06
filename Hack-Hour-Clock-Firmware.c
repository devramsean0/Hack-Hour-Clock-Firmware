#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "pico/cyw43_arch.h"
#include "sys/timeb.h"

#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi1
#define RCLK_PIN 8
#define CLK_PIN 10
#define MOSI_PIN 11

void GPIO_MODE(UWORD Pin, UWORD Mode) {
    gpio_init(Pin);
    if(Mode == 0 || Mode == GPIO_IN) {
        gpio_set_dir(Pin, GPIO_IN);
    } else {
        gpio_set_dir(Pin, GPIO_OUT);
    }
}

int init_8seg_led() {
    spi_init(SPI_PORT, 1000*1000);

    gpio_set_function(CLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MOSI_PIN, GPIO_FUNC_SPI);

    GPIO_MODE(RCLK_PIN, 1);
    gpio_put(RCLK_PIN, 1);

    if (spi_get_hw(SPI_PORT) == NULL) {
        return 0;
    }
    pico_8seg_led_send_command(0X57, 0x35);
    return 1;
};

void led_send_command(UBYTE const Num, UBYTE const Seg) {
    gpio_put(RCLK_PIN, 1);
    spi_write_blocking(SPI_PORT, &Num, 1);
    spi_write_blocking(SPI_PORT, &Seg, 1);
    gpio_put(RCLK_PIN, 0);
    sleep_ms(2);
    gpio_put(RCLK_PIN, 1);
}

int main()
{
    stdio_init_all();

    // Initialise the Wi-Fi chip
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }

    if (init_8seg_led) {
        printf("8-segment LED init success");
    }

    // Enable wifi station
    cyw43_arch_enable_sta_mode();

    printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms("Your Wi-Fi SSID", "Your Wi-Fi Password", CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        return 1;
    } else {
        printf("Connected.\n");
        // Read the ip address in a human readable way
        uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
        printf("IP address %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
    }

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
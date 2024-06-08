// Delay usb connection indefinitely
#ifndef PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS
#define PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS (-1)
#endif

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "pico/cyw43_arch.h"
#include "sys/timeb.h"

#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

#define str(s) # s
#define xstr(s) str(s)

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi1
#define RCLK_PIN 9
#define CLK_PIN 10
#define MOSI_PIN 11

UBYTE SEG8Code[] ={
        0x3F, // 0
        0x06, // 1
        0x5B, // 2
        0x4F, // 3
        0x66, // 4
        0x6D, // 5
        0x7D, // 6
        0x07, // 7
        0x7F, // 8
        0x6F, // 9
        0x77, // A
        0x7C, // b
        0x39, // C
        0x5E, // d
        0x79, // E
        0x71, // F
    };

#define KILOBIT   0XFE //KiloBit
#define HUNDREDS  0XFD //hundreds
#define TENS      0XFB //tens
#define UNITS     0XF7 //units
#define Dot       0x80

void GPIO_MODE(UWORD Pin, UWORD Mode) {
    gpio_init(Pin);
    if(Mode == 0 || Mode == GPIO_IN) {
        gpio_set_dir(Pin, GPIO_IN);
    } else {
        gpio_set_dir(Pin, GPIO_OUT);
    }
}

void led_send_command(UBYTE const Num, UBYTE const Seg) {
    gpio_put(RCLK_PIN, 1);
    spi_write_blocking(SPI_PORT, &Num, 1);
    spi_write_blocking(SPI_PORT, &Seg, 1);
    gpio_put(RCLK_PIN, 0);
    sleep_ms(2);
    gpio_put(RCLK_PIN, 1);
}

int init_8seg_led() {
    spi_init(SPI_PORT, 1000*1000);

    gpio_set_function(CLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MOSI_PIN, GPIO_FUNC_SPI);

    GPIO_MODE(RCLK_PIN, 1);
    gpio_put(RCLK_PIN, 1);
    return 1;
};

int main()
{
    _Bool result = stdio_usb_init();
    stdio_init_all();
    bool WIFI_CONNECTED = false;

    // Initialise the Wi-Fi chip
    printf("Connecting to Wi-Fi...\n");
    cyw43_arch_init();
    if (cyw43_arch_wifi_connect_timeout_ms("Gigaclear_C479", "x1izzxbnct", CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
    } else {
        uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
        printf("IP address %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
        printf("Connected.\n");
        WIFI_CONNECTED = true;
    }
    cyw43_arch_enable_sta_mode();

    if (init_8seg_led()) {
        printf("8-segment LED init success");
    } else {
        printf("8-segment LED init failed");
    }

    // Enable wifi station

    /* printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms("Your Wi-Fi SSID", "Your Wi-Fi Password", CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        return 1;
    } else {
        printf("Connected.\n");
        // Read the ip address in a human readable way
        uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
        printf("IP address %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
    } */

    while (true) {
        if (WIFI_CONNECTED) {
            led_send_command(KILOBIT, SEG8Code[1]);
        } else {
            led_send_command(KILOBIT, SEG8Code[0]);
        }
    }
}
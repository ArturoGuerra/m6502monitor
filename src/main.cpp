#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>

#include "io.h"

/*
Pin map:
0 = Clock GPIO
1 = RWB PIO
2 - 9 Data Bus PIO
10 - 22 Address Bus PIO
23 - 25 Discard data
26 - 28 Address Bus PIO
*/

#define CLOCK_PIN 0
#define BASE_IO_PIN 1

IO *io;

void gpio_callback(uint gpio, uint32_t event) {
    io_t io_data = io->fetch();
    printf("0x%04x 0x%02x %c\n", io_data.addressbus, io_data.databus, io_data.rwb ? 'R' : 'w');
}

int main() {
    stdio_init_all();
    
    PIO pio = pio1;
    int sm = pio_claim_unused_sm(pio, true);
    io = new IO(pio, sm, BASE_IO_PIN);
    
    gpio_init(CLOCK_PIN);
    gpio_set_dir(CLOCK_PIN, GPIO_IN);
    gpio_set_irq_enabled_with_callback(CLOCK_PIN, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);

    while(true) tight_loop_contents();
}
#include "io.h"
#include "io.pio.h"

#define RWB_OFFSET 0
#define RWB_MASK 0x1

#define DATABUS_OFFSET 1
#define DATABUS_MASK 0xFF

#define ADDRESSBUS_OFFSET 9
#define ADDRESSBUS_MASK 0x1FFFF
#define ADDRESSBUS_LEFT_OFFSET 12
#define ADDRESSBUS_LEFT_MASK 0xE000

#define BUS_LEN 28

static inline void io_init(PIO pio, int sm, uint offset, uint base_pin);

IO::IO(PIO pio, int sm, uint base_pin) {
    this->pio = pio;
    this->sm = sm;
    this->base_pin = base_pin;   
    
    uint offset = pio_add_program(pio, &iobus_program);

    io_init(pio, sm, offset, base_pin);
}

io_t IO::fetch() {
    uint16_t address;
    uint8_t data;
    bool rwb;
    fetch(address, data, rwb);
    return io_t{address, data, rwb};
}

void IO::fetch(addressbus_t& address, databus_t& data, bool& rwb) {
    uint32_t bus = fetch_raw();
    rwb = RWB_MASK & (bus >> RWB_OFFSET);
    data = DATABUS_MASK & (bus >> DATABUS_OFFSET);
    address = (ADDRESSBUS_LEFT_MASK & (bus >> ADDRESSBUS_LEFT_OFFSET)) | (ADDRESSBUS_MASK & (bus >> ADDRESSBUS_OFFSET));
}

uint32_t IO::fetch_raw() {
    return (uint32_t)pio_sm_get_blocking(pio, sm);
}

static inline void io_init(PIO pio, int sm, uint offset, uint base_pin) {
    for (uint i = 0; BUS_LEN > i; i++) {
        pio_gpio_init(pio, base_pin + i);
    }

    pio_sm_set_consecutive_pindirs(pio, sm, base_pin, BUS_LEN, false);
    
    pio_sm_config c = iobus_program_get_default_config(offset);

    sm_config_set_in_pins(&c, base_pin);
    sm_config_set_in_shift(&c, false, false, BUS_LEN);
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_RX);

    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}
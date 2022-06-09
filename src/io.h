#pragma once

#include <hardware/pio.h>


typedef uint8_t databus_t;
typedef uint16_t addressbus_t;

typedef struct io_s {
    addressbus_t addressbus;
    databus_t databus;
    bool rwb;
} io_t;

class IO {
private:
    PIO pio;
    int sm;
    uint base_pin;

public: 
    IO(PIO pio, int sm, uint base_pin);
    io_t fetch();
    void fetch(addressbus_t& address, databus_t& data, bool& rwb);
    uint32_t fetch_raw();
};
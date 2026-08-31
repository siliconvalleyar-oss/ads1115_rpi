#ifndef ADS1115_H
#define ADS1115_H

#include <cstdint>
#include <bcm2835.h>

class ADS1115 {
private:
    uint8_t address;
    uint8_t current_channel;

    uint16_t build_config(uint8_t channel);

public:
    ADS1115(uint8_t addr = 0x48);
    bool init();
    void close();

    bool set_channel(uint8_t channel);
    int16_t read_raw();
    float read_voltage();
};

#endif

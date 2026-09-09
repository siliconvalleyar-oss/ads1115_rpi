#ifndef ADS1115_H
#define ADS1115_H

#include <cstdint>

class ADS1115 {
private:
    uint8_t address;
    uint8_t current_channel;
    int fd;

    uint16_t build_config(uint8_t channel);
    bool i2c_write(const uint8_t *buf, uint32_t len);
    bool i2c_read(uint8_t *buf, uint32_t len);

public:
    ADS1115(uint8_t addr = 0x48);
    bool init();
    void close();

    bool set_channel(uint8_t channel);
    int16_t read_raw();
    float read_voltage();
};

#endif

#include "ads1115.h"
#include <iostream>
#include <unistd.h>

ADS1115::ADS1115(uint8_t addr) : address(addr), current_channel(0) {}

bool ADS1115::init() {
    if (!bcm2835_init()) {
        std::cerr << "Error al inicializar bcm2835" << std::endl;
        return false;
    }
    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(address);
    return true;
}

void ADS1115::close() {
    bcm2835_i2c_end();
    bcm2835_close();
}

uint16_t ADS1115::build_config(uint8_t channel) {
    uint16_t config = 0x8000; // OS: Single shot
    config |= (0x4000 | (channel << 12)); // MUX: AINx vs GND
    config |= 0x0000; // PGA: +/-6.144V
    config |= 0x0100; // Mode: Single shot
    config |= 0x0080; // DR: 128SPS
    config |= 0x0003; // CQUE: None
    return config;
}

bool ADS1115::set_channel(uint8_t channel) {
    if (channel > 3) return false;
    current_channel = channel;
    return true;
}

int16_t ADS1115::read_raw() {
    uint16_t config = build_config(current_channel);
    uint8_t buf[3];
    buf[0] = 0x01; // Config register
    buf[1] = (config >> 8) & 0xFF;
    buf[2] = config & 0xFF;

    bool ok = bcm2835_i2c_write(reinterpret_cast<const char *>(buf), 3) == BCM2835_I2C_REASON_OK;
    if (!ok) {
        std::cerr << "Error al escribir configuracion I2C" << std::endl;
        return 0;
    }
    usleep(8000); // Wait for conversion

    uint8_t reg = 0x00; // Conversion register
    uint8_t data[2];
    ok = bcm2835_i2c_write_read_rs(reinterpret_cast<char *>(&reg), 1, reinterpret_cast<char *>(data), 2) == BCM2835_I2C_REASON_OK;
    if (!ok) {
        std::cerr << "Error al leer conversion I2C" << std::endl;
        return 0;
    }

    return (data[0] << 8) | data[1];
}

float ADS1115::read_voltage() {
    int16_t raw = read_raw();
    return raw * 6.144f / 32768.0f;
}

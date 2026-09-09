#include "ads1115.h"
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

ADS1115::ADS1115(uint8_t addr) : address(addr), current_channel(0), fd(-1) {}

bool ADS1115::i2c_write(const uint8_t *buf, uint32_t len) {
    if (fd < 0) return false;
    ssize_t written = write(fd, buf, len);
    return written == static_cast<ssize_t>(len);
}

bool ADS1115::i2c_read(uint8_t *buf, uint32_t len) {
    if (fd < 0) return false;
    ssize_t read_bytes = read(fd, buf, len);
    return read_bytes == static_cast<ssize_t>(len);
}

bool ADS1115::init() {
    fd = open("/dev/i2c-1", O_RDWR);
    if (fd < 0) {
        perror("Error al abrir /dev/i2c-1");
        return false;
    }

    if (ioctl(fd, I2C_SLAVE, address) < 0) {
        perror("Error al configurar direccion I2C");
        ::close(fd);
        fd = -1;
        return false;
    }

    return true;
}

void ADS1115::close() {
    if (fd >= 0) {
        ::close(fd);
        fd = -1;
    }
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
    if (fd < 0) return 0;

    uint16_t config = build_config(current_channel);
    uint8_t buf[3];
    buf[0] = 0x01; // Config register
    buf[1] = (config >> 8) & 0xFF;
    buf[2] = config & 0xFF;

    if (!i2c_write(buf, 3)) {
        perror("Error al escribir configuracion I2C");
        return 0;
    }

    usleep(8000); // Wait for conversion

    uint8_t reg = 0x00; // Conversion register
    if (!i2c_write(&reg, 1)) {
        perror("Error al seleccionar registro conversion");
        return 0;
    }

    uint8_t data[2];
    if (!i2c_read(data, 2)) {
        perror("Error al leer datos conversion");
        return 0;
    }

    return (data[0] << 8) | data[1];
}

float ADS1115::read_voltage() {
    int16_t raw = read_raw();
    return raw * 6.144f / 32768.0f;
}

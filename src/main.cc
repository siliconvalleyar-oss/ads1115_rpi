#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

// Dirección I2C del ADS1115
#define ADS1115_ADDRESS 0x48

// Registros del ADS1115
#define ADS1115_REG_POINTER_CONVERT 0x00
#define ADS1115_REG_POINTER_CONFIG  0x01

// Configuración del ADS1115
#define ADS1115_CONFIG_OS_SINGLE    (0x8000)
#define ADS1115_CONFIG_MUX_SINGLE_0 (0x4000)
#define ADS1115_CONFIG_PGA_6_144V   (0x0000)
#define ADS1115_CONFIG_MODE_SINGLE  (0x0100)
#define ADS1115_CONFIG_DR_128SPS    (0x0080)
#define ADS1115_CONFIG_CQUE_NONE    (0x0003)

int main() {
    int file;
    const char *filename = "/dev/i2c-1";

    // Abre el bus I2C
    if ((file = open(filename, O_RDWR)) < 0) {
        std::cerr << "Error al abrir el bus I2C" << std::endl;
        return 1;
    }

    // Dirección del dispositivo I2C
    if (ioctl(file, I2C_SLAVE, ADS1115_ADDRESS) < 0) {
        std::cerr << "Error al comunicar con el dispositivo" << std::endl;
        close(file);
        return 1;
    }

    // Configuración del ADS1115 para una lectura de un solo disparo
    uint16_t config = ADS1115_CONFIG_OS_SINGLE | ADS1115_CONFIG_MUX_SINGLE_0 |
                      ADS1115_CONFIG_PGA_6_144V | ADS1115_CONFIG_MODE_SINGLE |
                      ADS1115_CONFIG_DR_128SPS | ADS1115_CONFIG_CQUE_NONE;

    uint8_t config_data[3] = {
        ADS1115_REG_POINTER_CONFIG,
        static_cast<uint8_t>(config >> 8),
        static_cast<uint8_t>(config & 0xFF)
    };

    // Escribe la configuración en el registro de configuración
    if (write(file, config_data, 3) != 3) {
        std::cerr << "Error al escribir la configuración" << std::endl;
        close(file);
        return 1;
    }

    // Espera el tiempo de conversión
    usleep(8000);

    // Dirección del registro de conversión
    uint8_t reg = ADS1115_REG_POINTER_CONVERT;
    if (write(file, &reg, 1) != 1) {
        std::cerr << "Error al seleccionar el registro de conversión" << std::endl;
        close(file);
        return 1;
    }

    // Lee los datos de la conversión
    uint8_t data[2];
    if (read(file, data, 2) != 2) {
        std::cerr << "Error al leer los datos" << std::endl;
        close(file);
        return 1;
    }

    // Convierte los datos a un valor de 16 bits
    int16_t result = (data[0] << 8) | data[1];

    // Imprime el resultado
    std::cout << "Valor leído: " << result << std::endl;

    // Cierra el archivo
    close(file);

    return 0;
}

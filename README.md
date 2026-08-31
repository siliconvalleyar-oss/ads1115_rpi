# ads1115_rpi

Libreria C++ para leer el ADC ADS1115 de Texas Instruments en Raspberry Pi usando la libreria `bcm2835`.

## Caracteristicas

- Comunicacion I2C via libreria `bcm2835`
- Lectura de los 4 canales single-ended (AIN0..AIN3)
- Conversion a voltaje
- Modo single-shot, 128 SPS, PGA +/-6.144V

## Hardware

| ADS1115 | Raspberry Pi |
|---------|--------------|
| VDD     | 3.3V o 5V    |
| GND     | GND          |
| SCL     | SCL (GPIO 3) |
| SDA     | SDA (GPIO 2) |
| ADDR    | GND (0x48)   |

Direcciones I2C segun ADDR:

| ADDR | Direccion |
|------|-----------|
| GND  | 0x48      |
| VDD  | 0x49      |
| SDA  | 0x4A      |
| SCL  | 0x4B      |

## Requisitos

- Raspberry Pi con Raspberry Pi OS
- I2C habilitado (`sudo raspi-config` -> Interfacing Options -> I2C)
- `libbcm2835-dev` instalado

## Instalacion

```bash
sudo apt update
sudo apt install libbcm2835-dev i2c-tools
```

Habilitar I2C y reiniciar:

```bash
sudo raspi-config
sudo reboot
```

Verificar dispositivo:

```bash
i2cdetect -y 1
```

Debe mostrar `48`.

## Compilacion

```bash
make clean
make -j4
```

## Ejecucion

```bash
sudo ./bin/ads1115
```

Salida esperada:

```
CANAL 0: 1234 (0.07 V)
CANAL 1: -456 (-0.03 V)
CANAL 2: 0 (0.00 V)
CANAL 3: 32767 (6.14 V)
```

## API

```cpp
#include "ads1115.h"

ADS1115 adc(0x48);           // Direccion I2C
adc.init();                  // Inicializa bcm2835 e I2C
adc.set_channel(0);          // Selecciona AIN0..AIN3
int16_t raw = adc.read_raw(); // Valor crudo de 16 bits
float voltage = adc.read_voltage(); // Voltaje en volts
adc.close();                 // Cierra I2C y bcm2835
```

## Estructura del proyecto

```
.
+-- include/
|   +-- ads1115.h
+-- src/
|   +-- main.cc
|   +-- ads1115.cc
+-- bin/
+-- obj/
+-- Makefile
+-- README.md
```

## Licencia

MIT

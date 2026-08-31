# Uso

## Compilacion

```bash
make clean
make -j4
```

## Ejecucion

Requiere permisos de acceso a I2C (ejecutar como root o con permisos adecuados):

```bash
sudo ./bin/ads1115
```

## Ejemplo de salida

```
CANAL 0: 1234 (0.07 V)
CANAL 1: -456 (-0.03 V)
CANAL 2: 0 (0.00 V)
CANAL 3: 32767 (6.14 V)
```

## API

```cpp
#include "ads1115.h"

ADS1115 adc(0x48); // Direccion I2C
adc.init();
adc.set_channel(0);
int16_t raw = adc.read_raw();
float voltage = adc.read_voltage();
adc.close();
```

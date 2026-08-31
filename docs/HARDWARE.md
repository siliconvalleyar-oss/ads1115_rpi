# Hardware

## Conexiones

| ADS1115 | Raspberry Pi |
|---------|--------------|
| VDD     | 3.3V o 5V    |
| GND     | GND          |
| SCL     | SCL (GPIO 3) |
| SDA     | SDA (GPIO 2) |
| ADDR    | GND (0x48)   |

## Direcciones I2C

| ADDR | Direccion |
|------|-----------|
| GND  | 0x48      |
| VDD  | 0x49      |
| SDA  | 0x4A      |
| SCL  | 0x4B      |

## Notas

- Usar resistencias de pull-up de 4.7k en SDA y SCL si el modulo no las incluye.
- Verificar que el I2C este habilitado en `raspi-config`.

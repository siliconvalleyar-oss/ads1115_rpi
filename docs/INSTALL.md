# Instalacion

## Requisitos

- Raspberry Pi con Raspberry Pi OS
- I2C habilitado en `raspi-config`
- Libreria bcm2835 instalada

## Instalar bcm2835

```bash
sudo apt update
sudo apt install libbcm2835-dev
```

## Habilitar I2C

```bash
sudo raspi-config
# Interfacing Options -> I2C -> Yes
sudo reboot
```

## Verificar dispositivo I2C

```bash
sudo apt install i2c-tools
i2cdetect -y 1
```

Deberia mostrar `48` en la salida si el ADS1115 esta conectado.

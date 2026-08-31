#include "ads1115.h"
#include <iostream>

int main() {
    ADS1115 adc(0x48);

    if (!adc.init()) {
        return 1;
    }

    for (int i = 0; i < 4; i++) {
        adc.set_channel(i);
        int16_t raw = adc.read_raw();
        float voltage = adc.read_voltage();
        std::cout << "CANAL " << i << ": " << raw << " (" << voltage << " V)" << std::endl;
    }

    adc.close();
    return 0;
}

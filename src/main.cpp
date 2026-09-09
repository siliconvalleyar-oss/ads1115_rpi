#include "ads1115.h"
#include <cstdio>
#include <unistd.h>

int main() {
    ADS1115 adc(0x48);

    if (!adc.init()) {
        return 1;
    }

    for (int i = 0; i < 4; i++) {
        adc.set_channel(i);
        int16_t raw = adc.read_raw();
        float voltage = adc.read_voltage();

        char buf[32];
        snprintf(buf, sizeof(buf), "Canal %d: %d (%.2f V)\n", i, raw, voltage);
        printf("%s", buf);
        usleep(500000);
    }

    adc.close();
    return 0;
}

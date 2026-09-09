#include "ads1115.h"
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cstdlib>

void save_log_csv(const char *filename, float v0, float v1, float v2, float v3) {
    FILE *f = fopen(filename, "a");
    if (!f) return;

    time_t now = time(nullptr);
    char timestr[64];
    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", localtime(&now));

    fprintf(f, "%s,%.4f,%.4f,%.4f,%.4f\n", timestr, v0, v1, v2, v3);
    fclose(f);
}

void show_menu() {
    printf("\n=== MENU ADS1115 ===\n");
    printf("1) Medir una vez\n");
    printf("2) Medir continuamente\n");
    printf("3) Salir\n");
    printf("Opcion: ");
}

int main() {
    ADS1115 adc(0x48);

    if (!adc.init()) {
        return 1;
    }

    int opcion = 0;
    bool running = true;

    while (running) {
        show_menu();
        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        if (opcion == 1) {
            for (int i = 0; i < 4; i++) {
                adc.set_channel(i);
                int16_t raw = adc.read_raw();
                float voltage = adc.read_voltage();
                printf("Canal %d: %d (%.2f V)\n", i, raw, voltage);
            }
        } else if (opcion == 2) {
            char logfile[128];
            time_t now = time(nullptr);
            char timestr[64];
            strftime(timestr, sizeof(timestr), "%Y%m%d_%H%M%S", localtime(&now));
            snprintf(logfile, sizeof(logfile), "log_ads1115_%s.csv", timestr);

            printf("Guardando log en: %s\n", logfile);
            printf("Presione '0' para detener...\n\n");

            bool measuring = true;
            int count = 0;
            while (measuring) {
                float v[4];
                for (int i = 0; i < 4; i++) {
                    adc.set_channel(i);
                    v[i] = adc.read_voltage();
                }

                printf("\033[5A\r");
                printf("\033[J");
                printf("Canal 0: %.2f V\n", v[0]);
                printf("Canal 1: %.2f V\n", v[1]);
                printf("Canal 2: %.2f V\n", v[2]);
                printf("Canal 3: %.2f V\n", v[3]);
                printf("\nPresione '0' para detener...\n");

                if (count % 10 == 0) {
                    save_log_csv(logfile, v[0], v[1], v[2], v[3]);
                }
                count++;

                usleep(200000);
                if (getchar() == '0') {
                    measuring = false;
                }
            }
        } else if (opcion == 3) {
            running = false;
        }
    }

    adc.close();
    return 0;
}

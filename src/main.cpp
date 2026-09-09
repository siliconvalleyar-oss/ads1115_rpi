#include "ads1115.h"
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <ncurses.h>
#include <sys/stat.h>

void save_log_csv(const char *filename, float v0, float v1, float v2, float v3) {
    mkdir("logs", 0777);
    char path[256];
    snprintf(path, sizeof(path), "logs/%s", filename);
    FILE *f = fopen(path, "a");
    if (!f) return;

    time_t now = time(nullptr);
    char timestr[64];
    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", localtime(&now));

    fprintf(f, "%s,%.4f,%.4f,%.4f,%.4f\n", timestr, v0, v1, v2, v3);
    fclose(f);
}

void draw_menu(WINDOW *menu_win) {
    werase(menu_win);
    mvwprintw(menu_win, 0, 0, "=== MENU ADS1115 ===");
    mvwprintw(menu_win, 1, 0, "1) Medir una vez");
    mvwprintw(menu_win, 2, 0, "2) Medir continuamente");
    mvwprintw(menu_win, 3, 0, "3) Salir");
    mvwprintw(menu_win, 4, 0, "Opcion: ");
    wrefresh(menu_win);
}

void draw_measurements(WINDOW *meas_win, float v0, float v1, float v2, float v3) {
    werase(meas_win);
    mvwprintw(meas_win, 0, 0, "Canal 0: %.4f V", v0);
    mvwprintw(meas_win, 1, 0, "Canal 1: %.4f V", v1);
    mvwprintw(meas_win, 2, 0, "Canal 2: %.4f V", v2);
    mvwprintw(meas_win, 3, 0, "Canal 3: %.4f V", v3);
    wrefresh(meas_win);
}

int main(int argc, char *argv[]) {
    bool auto_mode = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--auto") == 0 || strcmp(argv[i], "-a") == 0) {
            auto_mode = true;
        }
    }

    ADS1115 adc(0x48);

    if (!adc.init()) {
        fprintf(stderr, "Error: no se pudo inicializar ADS1115 en 0x48\n");
        fprintf(stderr, "Verifique conexion I2C y ejecute con sudo.\n");
        return 1;
    }

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    int meas_height = 8;
    int menu_height = 8;
    int meas_width = 40;
    int menu_width = 40;
    int meas_y = 0;
    int menu_y = meas_height + 1;

    WINDOW *meas_win = newwin(meas_height, meas_width, meas_y, 0);
    WINDOW *menu_win = newwin(menu_height, menu_width, menu_y, 0);
    box(meas_win, 0, 0);
    box(menu_win, 0, 0);

    bool running = true;
    bool measuring = false;
    int count = 0;
    char logfile[128] = {0};

    if (auto_mode) {
        measuring = true;
        time_t now = time(nullptr);
        char timestr[64];
        strftime(timestr, sizeof(timestr), "%Y%m%d_%H%M%S", localtime(&now));
        snprintf(logfile, sizeof(logfile), "log_ads1115_%s.csv", timestr);
    }

    while (running) {
        if (!measuring && !auto_mode) {
            draw_menu(menu_win);
        }

        int ch = getch();
        if (!auto_mode) {
            if (ch == '3' || ch == 'q' || ch == 'Q') {
                running = false;
                break;
            } else if (ch == '1') {
                measuring = true;
                float v[4] = {0};
                for (int i = 0; i < 4; i++) {
                    adc.set_channel(i);
                    v[i] = adc.read_voltage();
                }
                draw_measurements(meas_win, v[0], v[1], v[2], v[3]);
                measuring = false;
            } else if (ch == '2') {
                if (logfile[0] == '\0') {
                    time_t now = time(nullptr);
                    char timestr[64];
                    strftime(timestr, sizeof(timestr), "%Y%m%d_%H%M%S", localtime(&now));
                    snprintf(logfile, sizeof(logfile), "log_ads1115_%s.csv", timestr);
                }
                measuring = true;
            }
        }

        if (measuring) {
            float v[4];
            for (int i = 0; i < 4; i++) {
                adc.set_channel(i);
                v[i] = adc.read_voltage();
            }

            draw_measurements(meas_win, v[0], v[1], v[2], v[3]);

            if (count % 10 == 0 && logfile[0] != '\0') {
                save_log_csv(logfile, v[0], v[1], v[2], v[3]);
            }
            count++;

            napms(200);
        }
    }

    delwin(meas_win);
    delwin(menu_win);
    endwin();

    adc.close();
    return 0;
}

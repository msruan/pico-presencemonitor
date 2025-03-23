#include <stdio.h>
#include "pico/stdlib.h"
#include "./utils.h"
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

// Define o pino do sensor PIR

#define HIGH 1
#define LOW 0

int setup_pir_sensor(){
    #ifndef PIR_SENSOR_PIN
        #define PIR_SENSOR_PIN 8
    #endif

    gpio_init(PIR_SENSOR_PIN);
    gpio_set_dir(PIR_SENSOR_PIN, GPIO_IN);

    // Ativa pull-down para evitar ruídos
    gpio_set_pulls(PIR_SENSOR_PIN, false, true);

    printf("Inicializando sensor PIR...\n");
    sleep_ms(1000);
    return PICO_OK;
}


int main() {
    stdio_init_all();
    hard_assert(setup_pir_sensor() ==  PICO_OK);
    hard_assert(setup_wifi() ==  PICO_OK);

    int last_state = LOW;
    
    while (true) {
        int pir_state = gpio_get(PIR_SENSOR_PIN);

          // Apenas detecta a transição LOW -> HIGH
        if (pir_state == HIGH && last_state == LOW) {
            send_http_post();
            cyw43_arch_poll();
            printf("Someone moved!\n");
            last_state = HIGH;
            sleep_ms(2500);
        }

        if (pir_state == LOW) {
            last_state = LOW;
        }
        sleep_ms(100);
    }
}

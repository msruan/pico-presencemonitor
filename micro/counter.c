#include <stdio.h>
#include "pico/stdlib.h"
#include "./utils.h"
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

// Define o pino do sensor PIR

#define HIGH 1
#define LOW 0

int main() {
    stdio_init_all();
    hard_assert(setup_led() ==  PICO_OK);
    hard_assert(setup_pir_sensor() ==  PICO_OK);
    hard_assert(setup_wifi() ==  PICO_OK);
    blink_led(PICO_LED_GREEN_PIN, 1000);

    int last_state = LOW;
    
    while (true) {
        int pir_state = gpio_get(PIR_SENSOR_PIN);

          // Apenas detecta a transição LOW -> HIGH
        if (pir_state == HIGH && last_state == LOW) {
            blink_led(PICO_LED_BLUE_PIN, 500);
            printf("Alguém se mexeu!\n");
            send_http_post();
            cyw43_arch_poll();
            last_state = HIGH;
            sleep_ms(2500);
        }

        if (pir_state == LOW) {
            last_state = LOW;
        }
        sleep_ms(100);
    }
}

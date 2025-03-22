#include <stdio.h>
#include "pico/stdlib.h"
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

// Define o pino do sensor PIR
#define PIR_SENSOR_PIN 16  // Ajuste para o pino correto da BitDogLab
#define PIR2_SENSOR_PIN 8 // Ajuste para o pino correto da BitDogLab

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 250
#endif

// Inicializa o LED corretamente, seja na Pico normal ou Pico W
int pico_led_init(void) {
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    return cyw43_arch_init();
#endif
}

// Liga ou desliga o LED
void pico_set_led(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}

int main() {
    stdio_init_all();
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    
    // Configura o pino do sensor PIR como entrada
    gpio_init(PIR_SENSOR_PIN);
    gpio_set_dir(PIR_SENSOR_PIN, GPIO_IN);
    
    printf("Inicializando sensor PIR...\n");
    
    while (true) {
        int pir_state = gpio_get(PIR_SENSOR_PIN); // Lê o estado do sensor
        int pir2_state = gpio_get(PIR2_SENSOR_PIN); // Lê o estado do sensor

        if (pir2_state == 1) {
            printf("Someone moved on left!\n");
        }
        else{
            printf("Monitoring left...\n");
        }
        
        if (pir_state == 1) {
            printf("Someone moved on right!\n");
        }
        else{
            printf("Monitoring right...\n");
        }
        sleep_ms(1000); // Aguarda um segundo antes de verificar novamente
    }
}

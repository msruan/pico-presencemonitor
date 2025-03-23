#include <stdio.h>
#include "pico/stdlib.h"
#include "./utils.h"
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

// Define o pino do sensor PIR
#define PIR_SENSOR_PIN 8 // Ajuste para o pino correto da BitDogLab

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 250
#endif

#define HIGH 1
#define LOW 0

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
    
    if(!setup_wifi()){
        printf("Erro ao inicializar o Wi-Fi!");
    }
    else{
        for (size_t i = 0; i < 10; i++)
        {
           printf("ok");
           sleep_ms(1000);
        }
        
    }
    
    // Configura o pino do sensor PIR como entrada
    gpio_init(PIR_SENSOR_PIN);
    gpio_set_dir(PIR_SENSOR_PIN, GPIO_IN);
    gpio_set_pulls(PIR_SENSOR_PIN, false, true);  // Ativa pull-down para evitar ruídos

    printf("Inicializando sensor PIR...\n");

    int last_state = LOW;
    
    while (true) {
        int pir_state = gpio_get(PIR_SENSOR_PIN);

        if (pir_state == HIGH && last_state == LOW) {  // Apenas detecta a transição LOW -> HIGH
            printf("Someone moved!\n");
            last_state = HIGH;  // Atualiza estado
            sleep_ms(2500);  // Tempo de espera para evitar leituras repetidas
        }

        if (pir_state == LOW) {
            last_state = LOW;  // Reseta estado quando não há movimento
        }
        sleep_ms(100); // Aguarda um segundo antes de verificar novamente
    }
}

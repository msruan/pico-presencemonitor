#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "cJSON.h"

// Pinos utilizados para os LEDs
#define PICO_LED_GREEN_PIN 11
#define PICO_LED_BLUE_PIN 12
#define PICO_LED_RED_PIN 13

// Configurações de Wi-Fi (alterar para o SSID e senha da sua rede)
#define WIFI_SSID "DEUS E FIEL"
#define WIFI_PASSWORD "89banana"

// Configurações de Wi-Fi (alterar para o IP onde está rodando o Express e a porta)
#define SERVER_IP "192.168.0.12"
#define SERVER_PORT 3001

// Configurações do dashboard
#define BOARD_ID "1"
#define LOCATION "Bedroom"

// Configurações do TCP/IP
char request_buffer[512];
struct tcp_pcb *client_pcb;

err_t http_connected_callback(void *arg, struct tcp_pcb *pcb, err_t err)
{
    if (err != ERR_OK)
    {
        printf("Erro ao conectar ao servidor: %d\n", err);
        return err;
    }

    tcp_write(pcb, request_buffer, strlen(request_buffer), TCP_WRITE_FLAG_COPY);
    tcp_output(pcb);

    return ERR_OK;
}


// Enviar requisição PUT
void send_http_post()
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "sensor_id", BOARD_ID);
    cJSON_AddStringToObject(json, "location", LOCATION);
    char *json_str = cJSON_Print(json);
    cJSON_Delete(json);

    snprintf(request_buffer, sizeof(request_buffer),
             "POST /motion HTTP/1.1\r\n"
             "Host: " SERVER_IP "\r\n"
             "Content-Type: application/json\r\n"
             "Content-Length: %d\r\n"
             "Connection: close\r\n\r\n"
             "%s",
             (int)strlen(json_str), json_str);

    ip_addr_t server_addr;
    ip4addr_aton(SERVER_IP, &server_addr);

    client_pcb = tcp_new();
    if (!client_pcb)
    {
        printf("Erro ao criar PCB\n");
        return;
    }

    tcp_connect(client_pcb, &server_addr, SERVER_PORT, http_connected_callback);
}

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



int setup_led(){
    
    
    gpio_init(PICO_LED_GREEN_PIN);
    gpio_init(PICO_LED_BLUE_PIN);
    gpio_init(PICO_LED_RED_PIN);
    gpio_set_dir(PICO_LED_GREEN_PIN, GPIO_OUT);
    gpio_set_dir(PICO_LED_BLUE_PIN, GPIO_OUT);
    gpio_set_dir(PICO_LED_RED_PIN, GPIO_OUT);
    
    printf("Inicializando LED...\n");
    sleep_ms(1000);
    return PICO_OK;
}

/**
 * @brief Acende e apaga o LED vermelho da placa em um intervalo de tempo específico.
 * @param duration O tempo (em milissegundos) em que o LED ficará aceso. Se o valor
 *                 for `-1`, será utilizado o intervalo padrão de 500 ms.
 * @note Esta função assume que o pino do LED foi previamente configurado
 */
void blink_led(int color, int duration){
    #ifndef PICO_DEFAULT_LED_BLINK_INTERVAL
    #define PICO_DEFAULT_LED_BLINK_INTERVAL 500
    #endif
    
    gpio_put(color, 1);
    sleep_ms(duration != -1 ? duration : PICO_DEFAULT_LED_BLINK_INTERVAL);
    gpio_put(color, 0);
}

int setup_wifi()
{
    if (cyw43_arch_init())
    {
        printf("Falha na inicialização do Wi-Fi\n");
        return -1;
    }

    cyw43_arch_enable_sta_mode();

    printf("Conectando ao Wi-Fi...\n");

    // Tenta conectar ao Wi-Fi com o SSID e senha fornecidos
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000))
    {
        printf("Falha durante a conexão.\n");
        return 1;
    }
    printf("Conexão Wi-Fi bem sucedida.\n");
    sleep_ms(1000);

    // Obtém o endereço IP do dispositivo
    uint8_t *ip_address = (uint8_t *)&(cyw43_state.netif[0].ip_addr.addr);
    printf("Endereço IP %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
    return PICO_OK;
}
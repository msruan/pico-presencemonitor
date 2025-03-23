#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "cJSON.h"

// Configurações de Wi-Fi (alterar para o SSID e senha da sua rede)
#define WIFI_SSID "DEUS E FIEL"
#define WIFI_PASSWORD "89banana"

// Configurações de Wi-Fi (alterar para o IP onde está rodando o Express e a porta)
#define SERVER_IP "192.168.0.12"
#define SERVER_PORT 3000

#define PICO_DEFAULT_LED_PIN 11

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


void send_http_post()
{
    snprintf(request_buffer, sizeof(request_buffer),
             "POST /movements HTTP/1.1\r\n"
             "Host: " SERVER_IP "\r\n"
             "Connection: close\r\n\r\n");

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


int setup_wifi()
{
    // // Inicializa a interface de entrada/saída padrão
    // // stdio_init_all();

    // // Configura o pino do LED como saída
    // setup_led(PICO_DEFAULT_LED_PIN);

    // Inicializa o Wi-Fi
    if (cyw43_arch_init())
    {
        printf("Wi-Fi init failed\n");
        return -1;
    }

    // Habilita o modo Station (STA) do Wi-Fi
    cyw43_arch_enable_sta_mode();

    printf("Connecting to Wi-Fi...\n");

    // Tenta conectar ao Wi-Fi com o SSID e senha fornecidos
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000))
    {
        printf("Failed to connect.\n");
        return 1;
    }
    printf("Connected.\n");
    sleep_ms(1000);

    // Obtém o endereço IP do dispositivo
    uint8_t *ip_address = (uint8_t *)&(cyw43_state.netif[0].ip_addr.addr);
    printf("IP address %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
}

// int main()
// {
//     setup();
//     while (true)
//     {
//         cyw43_arch_poll(); // Processa eventos de rede
//         sleep_ms(500);
//     }
// }
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>

// Define UART handle (assuming you're using UART1)
extern UART_HandleTypeDef huart1;  // This should be configured in STM32CubeMX

// Function prototypes
void SystemClock_Config(void);
void UART_Init(void);
void send_at_command(char *command, char *response, uint16_t timeout);
void join_lora_network(void);
void UART_Error_Handler(void);
void UART_Receive_IT_Handler(void);

// Buffer for responses
char response[256];

// Main function
int main(void)
{
    // HAL initialization
    HAL_Init();
    SystemClock_Config();

    // UART Initialization
    UART_Init();

    // Start the OTAA join process
    join_lora_network();

    // Infinite loop
    while (1) {
        // Application code, e.g., handle sending/receiving LoRaWAN data
    }
}

// Function to initialize UART
void UART_Init(void)
{
    HAL_UART_Init(&huart1);
}

// Function to send AT command and receive response
void send_at_command(char *command, char *response, uint16_t timeout)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)command, strlen(command), timeout);
    HAL_UART_Receive(&huart1, (uint8_t*)response, 255, timeout);  // Receive response (max 255 chars)
}

// Function to join LoRa network using OTAA
void join_lora_network(void)
{
    // Set Device EUI (replace with your actual Device EUI)
    send_at_command("AT+DEVEUI=YOUR_DEVICE_EUI\r\n", response, 1000);
    HAL_Delay(500);

    // Set Application EUI (replace with your actual Application EUI)
    send_at_command("AT+APPEUI=YOUR_APP_EUI\r\n", response, 1000);
    HAL_Delay(500);

    // Set Application Key (replace with your actual Application Key)
    send_at_command("AT+APPKEY=YOUR_APP_KEY\r\n", response, 1000);
    HAL_Delay(500);

    // Set the mode to OTAA
    send_at_command("AT+MODE=OTAA\r\n", response, 1000);
    HAL_Delay(500);

    // Start the join process
    send_at_command("AT+JOIN\r\n", response, 3000);
    HAL_Delay(500);

    // Check if joined successfully
    if (strstr(response, "joined")) {
        // Successful join
        printf("Device successfully joined the network!\n");
    } else {
        // Join failed
        printf("Join failed. Response: %s\n", response);
    }
}

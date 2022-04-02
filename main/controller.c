#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "time.h"
#include "include/controller.h"

#define GPIO_BTN_START  4
#define GPIO_BTN_STOP   34

// Retornos de estado para os botões 
#define IDLE     0x00
#define START    0x01
#define STOP     0x02
#define CONFIRM  0X03
#define CONFIG   0x04
#define CANCEL   0x05

// Eventos duranre a utilização do timer

#define EVENT_WAIT_MODE     0x00
#define EVENT_START_MODE    0x01
#define EVENT_STOP_MODE     0x02
#define EVENT_RESULT_MODE   0x03
#define EVENT_CONFIG        0x04
#define EVENT_LIST_PLAYERS  0x05
#define EVENT_INIT_DEVICE   0x06

#define MIN_TIME_WAIT  500
#define MAX_TIME_WAIT  2000


QueueHandle_t xQueueController = NULL; 

bool flag_stop = false;
bool flag_start = false;
bool status;

void config_button(void)
{
    gpio_set_direction(GPIO_BTN_START, GPIO_MODE_INPUT);
    gpio_set_direction(GPIO_BTN_STOP, GPIO_MODE_INPUT);
}

uint64_t millis(){
    return (esp_timer_get_time()/1000);
}


uint8_t read_btn(void)
{
    static uint64_t start_press ;
    static uint64_t end_press ;
    
    if(!gpio_get_level(GPIO_BTN_START) && !flag_start)
    {
        start_press = millis();
        flag_start = 0x01;
    }
    else if(gpio_get_level(GPIO_BTN_START) && flag_start)
    {   
        end_press = millis()- start_press;
        
        if(end_press > MIN_TIME_WAIT )
        {
            flag_start = 0x00;
            return CONFIG;
        }
        else if(end_press > MAX_TIME_WAIT)
        {
            flag_start = 0x00;
            return CONFIRM;
        }        
        
        flag_start = 0x00;
        return START;
    }
    
    if(!gpio_get_level(GPIO_BTN_STOP) && !flag_stop)
    {
        flag_stop = 0x01;
        start_press = millis();
    }
    else if(gpio_get_level(GPIO_BTN_STOP) && flag_stop)
    {
        end_press = millis()- start_press;
        if(end_press > MIN_TIME_WAIT )
        {
            flag_stop = 0x00;
            return CANCEL;
        }
        flag_stop = 0x00;
        return STOP;     
    }
    return IDLE;
}

void vTask_controller (void *pvParameters)
{
uint8_t queue_data = 0x00 ;
uint8_t cmd = 0x00;
    while(1)
    {
        cmd = read_btn();
        if(xQueueReceive(xQueueController, &queue_data, 0) == pdTRUE)
        {
            switch(queue_data)
            {
                case EVENT_WAIT_MODE:// Aguarda na tela inicial
                break;

                case EVENT_START_MODE: // Inicia o processo de contagem do timer
                break;

                case EVENT_STOP_MODE: //Aguarda o usuário a dar prosseguimento ao temporizador ou vai direto para a tela de resultado
                break;

                case EVENT_RESULT_MODE: // Mostra o tempo do primeiro tiro e do ultimo tiro
                break;
                
                case EVENT_CONFIG: // Configura dispositivo na rede via bluetooth
                break;

                case EVENT_LIST_PLAYERS: // Mostra a lista  com os 10 ultimos tempos 
                break;

                case EVENT_INIT_DEVICE:
                // Regata a lista de tempos dos jogadores
                // Aviso sonoro de inicialização do dispositivo 
                // Verifica se o dispositivo esta conectado a internet 
                break;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void init_controller()
{
    xQueueController = xQueueCreate(5,sizeof(uint8_t));
    config_button();
    xTaskCreate(vTask_controller, "Controller", 4092, NULL, 2, NULL);
}

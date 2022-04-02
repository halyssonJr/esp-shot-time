#include <stdio.h>
#include <stdlib.h>
#include "esp_log.h"
#include "esp_adc_cal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"

#include "include/sensor.h"

bool check_adc = false;
int mic_gpio = ADC2_CHANNEL_6; 
static esp_adc_cal_characteristics_t adc1_chars;
static esp_adc_cal_characteristics_t adc2_chars;

static bool calibration_init(void)
{
    
    esp_err_t check;
    bool cali_enable = false;

    check = esp_adc_cal_check_efuse(ADC_EXAMPLE_CALI_SCHEME);
    if (check == ESP_ERR_NOT_SUPPORTED) {
        ESP_LOGW("WARNIG", "Configuração não suportada pelo chip");

    } else if (check == ESP_ERR_INVALID_VERSION) {
        ESP_LOGW("WARNIG", "Configuração para versão errada ou dispositivo errado");

    } else if (check == ESP_OK) {
        cali_enable = true;
        ESP_LOGI("INFO", "Dispositivo Configurado");
        esp_adc_cal_characterize(ADC_UNIT_2, ADC_ATTEN_11db, ADC_WIDTH_BIT_DEFAULT, 0, &adc2_chars);
        
    } else {
        ESP_LOGW("WARNIG", "Argumento inválido");
    }
        
        return cali_enable;
}



double get_value_sensor(void)
{ 
    double voltage = ((double)adc1_get_raw(mic_gpio)*2450/4095); 
    voltage = voltage/1000;  
   return voltage;
}

void vTask_microphone(void *pvParameters)
{
    while(1)
    {
        
    }

}
void init_sensor(void)
{
   calibration_init();
   adc2_config_channel_atten(mic_gpio, ADC_ATTEN_11db);
    
}
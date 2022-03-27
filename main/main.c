#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "include/ssd1306.h"
#include "include/font8x8_basic.h"


void app_main(void)
{
	SSD1306_t dev;
	int center, top, bottom;
	char lineChar[20];

	
	i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
	ssd1306_init(&dev, 128, 32);
	ssd1306_clear_screen(&dev, false);
	ssd1306_contrast(&dev, 0xff);

	top = 1;
	center = 1;
	bottom = 0;
	ssd1306_display_text(&dev, 1, "   Shot Time   ", 15, true);
	ssd1306_display_text(&dev, 3, "> Press Start <", 15, false);
	vTaskDelay(5000 / portTICK_PERIOD_MS);
	

	ssd1306_clear_screen(&dev, false);
	ssd1306_contrast(&dev, 0xff);
	ssd1306_display_text(&dev, 0, "--LAST SHOTS--", 14, true);
	//ssd1306_software_scroll(&dev, 1, 7);
	ssd1306_software_scroll(&dev, 1, (dev._pages - 1) );
	for (int line=0;line<bottom+5;line++) {
		lineChar[0] = 0x02;
		sprintf(&lineChar[1], "Shot%02d:", line);
		ssd1306_scroll_text(&dev, lineChar, strlen(lineChar), false);
		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
	vTaskDelay(3000 / portTICK_PERIOD_MS);

	ssd1306_clear_screen(&dev, true);
	ssd1306_contrast(&dev, 0xff);
	ssd1306_display_text(&dev, center, "  Finish!!", 12, true);
	vTaskDelay(5000 / portTICK_PERIOD_MS);

	ssd1306_fadeout(&dev);
	
	esp_restart();
}

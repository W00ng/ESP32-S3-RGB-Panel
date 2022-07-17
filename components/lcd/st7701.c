/*
 * SPDX-FileCopyrightText: 2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "st7701.h"

static const char *TAG = "st7701";

#if  0
    spi_device_handle_t spi;
    spi_bus_config_t buscfg={
        .miso_io_num = GPIO_NUM_NC,
        .mosi_io_num = GPIO_LCD_SDA,
        .sclk_io_num = GPIO_LCD_SCK,
        .quadwp_io_num = GPIO_NUM_NC,
        .quadhd_io_num = GPIO_NUM_NC,
        .max_transfer_sz = 1024,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));

    spi_device_interface_config_t devcfg={
        .spics_io_num = GPIO_LCD_CS,               //CS pin        
        .clock_speed_hz = 10*1000*1000,           //Clock out at 10 MHz
        .mode = 0,                                //SPI mode 0
        .command_bits = 8,           //< Default amount of bits in command phase (0-16), used when ``SPI_TRANS_VARIABLE_CMD`` is not used, otherwise ignored.
        .address_bits = 8,  
        .queue_size = 10,            //We want to be able to queue 7 transactions at a time
    };
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &devcfg, &spi));

    // esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &io_handle);
    // esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle);
#endif
static void spi_soft_init(void)
{
#if 0
    gpio_pad_select_gpio(GPIO_LCD_CS);
	gpio_set_direction(GPIO_LCD_CS, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_LCD_CS, 1);   

	gpio_pad_select_gpio(GPIO_LCD_SCK);
	gpio_set_direction(GPIO_LCD_SCK, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_LCD_SCK, 1);  

	gpio_pad_select_gpio(GPIO_LCD_SDA);
	gpio_set_direction(GPIO_LCD_SDA, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_LCD_SDA, 1);  
#else
    gpio_config_t io_conf1 = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << GPIO_LCD_CS, 
    };
    gpio_config(&io_conf1);
    gpio_set_level(GPIO_LCD_CS, 1);

    gpio_config_t io_conf2 = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << GPIO_LCD_SCK, 
    };
    gpio_config(&io_conf2);
    gpio_set_level(GPIO_LCD_SCK, 1);

    gpio_config_t io_conf3 = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << GPIO_LCD_SDA,
    };
    gpio_config(&io_conf3);
    gpio_set_level(GPIO_LCD_SDA, 1);
#endif
}

static void spi_soft_write_9bits(uint16_t data)
{
	uint8_t i;
	LCD_CS_Clr();
	for(i = 0; i < 9; i++)
	{
        LCD_SCK_Clr();
		if(data & 0x100)   LCD_SDA_Set();
		else               LCD_SDA_Clr();
		// delay_us(30);
        LCD_SCK_Set();
		// delay_us(30);
		data <<= 1;
	}
	LCD_CS_Set();;
}

static void st7701_write_cmd(uint8_t cmd)
{
	uint16_t temp = 0;
	temp = temp | cmd;
	spi_soft_write_9bits(temp);
}

static void st7701_write_data(uint8_t data)
{
	uint16_t temp = 0x100;
	temp = temp | data;
	spi_soft_write_9bits(temp);
}

void st7701_reg_init(void)
{
    spi_soft_init();   //GPIO init
    ESP_LOGI(TAG, "st7701 register init");

#if 0
    st7701_write_cmd(0xFF);
    st7701_write_data(0x77);
    st7701_write_data(0x01);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x10);

    st7701_write_cmd(0xC0);
    st7701_write_data(0x3B);    // Scan line	
    st7701_write_data(0x00);

    st7701_write_cmd(0xC1);
    st7701_write_data(0x0B);	// VBP
    st7701_write_data(0x02);

    st7701_write_cmd(0xC2);
    st7701_write_data(0x07);
    st7701_write_data(0x02);

    st7701_write_cmd(0xCC);
    st7701_write_data(0x10);

    st7701_write_cmd(0xCD);  // RGB format
    st7701_write_data(0x08);     

    st7701_write_cmd(0xB0); // IPS   
    st7701_write_data(0x00); // 255 
    st7701_write_data(0x11); // 251    
    st7701_write_data(0x16); // 247  down
    st7701_write_data(0x0e); // 239    
    st7701_write_data(0x11); // 231    
    st7701_write_data(0x06); // 203    
    st7701_write_data(0x05); // 175 
    st7701_write_data(0x09); // 147    
    st7701_write_data(0x08); // 108    
    st7701_write_data(0x21); // 80  
    st7701_write_data(0x06); // 52   
    st7701_write_data(0x13); // 24    
    st7701_write_data(0x10); // 16    
    st7701_write_data(0x29); // 8    down
    st7701_write_data(0x31); // 4    
    st7701_write_data(0x18); // 0   

    st7701_write_cmd(0xB1);//  IPS	   
    st7701_write_data(0x00);//  255 
    st7701_write_data(0x11);//  251
    st7701_write_data(0x16);//  247   down
    st7701_write_data(0x0e);//  239
    st7701_write_data(0x11);//  231
    st7701_write_data(0x07);//  203    
    st7701_write_data(0x05);//  175
    st7701_write_data(0x09);//  147  
    st7701_write_data(0x09);//  108  
    st7701_write_data(0x21);//  80 
    st7701_write_data(0x05);//  52   
    st7701_write_data(0x13);//  24 
    st7701_write_data(0x11);//  16 
    st7701_write_data(0x2a);//  8  down 
    st7701_write_data(0x31);//  4  
    st7701_write_data(0x18);//  0  

    st7701_write_cmd(0xFF);
    st7701_write_data(0x77);
    st7701_write_data(0x01);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x11);

    st7701_write_cmd(0xB0);     // VOP  3.5375+ *x 0.0125
    st7701_write_data(0x6d);    // 5D
    
    st7701_write_cmd(0xB1); 	// VCOM amplitude setting  
    st7701_write_data(0x37);    //
    
    st7701_write_cmd(0xB2); 	// VGH Voltage setting  
    st7701_write_data(0x81);	// 12V

    st7701_write_cmd(0xB3);
    st7701_write_data(0x80);

    st7701_write_cmd(0xB5); 	// VGL Voltage setting  
    st7701_write_data(0x43);	// -8.3V

    st7701_write_cmd(0xB7);
    st7701_write_data(0x85);

    st7701_write_cmd(0xB8);
    st7701_write_data(0x20);

    st7701_write_cmd(0xC1);
    st7701_write_data(0x78);

    st7701_write_cmd(0xC2);
    st7701_write_data(0x78);

    st7701_write_cmd(0xD0);
    st7701_write_data(0x88);

    st7701_write_cmd(0xE0);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x02);

    st7701_write_cmd(0xE1);
    st7701_write_data(0x03);	
    st7701_write_data(0xA0);
    st7701_write_data(0x00);	
    st7701_write_data(0x00);
    st7701_write_data(0x04);	
    st7701_write_data(0xA0);
    st7701_write_data(0x00);	
    st7701_write_data(0x00);	
    st7701_write_data(0x00);
    st7701_write_data(0x20);
    st7701_write_data(0x20);

    st7701_write_cmd(0xE2);
    st7701_write_data(0x00);	
    st7701_write_data(0x00);	  
    st7701_write_data(0x00);	
    st7701_write_data(0x00);	
    st7701_write_data(0x00);	
    st7701_write_data(0x00);	
    st7701_write_data(0x00);	
    st7701_write_data(0x00);	
    st7701_write_data(0x00);	
    st7701_write_data(0x00);	
    st7701_write_data(0x00);	
    st7701_write_data(0x00);	
    st7701_write_data(0x00);

    st7701_write_cmd(0xE3);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x11);
    st7701_write_data(0x00);

    st7701_write_cmd(0xE4);
    st7701_write_data(0x22);
    st7701_write_data(0x00);

    st7701_write_cmd(0xE5);		
    st7701_write_data(0x05);	
    st7701_write_data(0xEC);	
    st7701_write_data(0xA0);
    st7701_write_data(0xA0);
    st7701_write_data(0x07);	
    st7701_write_data(0xEE);	
    st7701_write_data(0xA0);
    st7701_write_data(0xA0);
    st7701_write_data(0x00);
    st7701_write_data(0x00);	
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);	
    st7701_write_data(0x00);
    st7701_write_data(0x00);

    st7701_write_cmd(0xE6);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x11);
    st7701_write_data(0x00);

    st7701_write_cmd(0xE7);
    st7701_write_data(0x22);
    st7701_write_data(0x00);

    st7701_write_cmd(0xE8);		
    st7701_write_data(0x06);	
    st7701_write_data(0xED);	
    st7701_write_data(0xA0);
    st7701_write_data(0xA0);
    st7701_write_data(0x08);	
    st7701_write_data(0xEF);	
    st7701_write_data(0xA0); 
    st7701_write_data(0xA0);
    st7701_write_data(0x00);	
    st7701_write_data(0x00);	
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);	
    st7701_write_data(0x00);	
    st7701_write_data(0x00);
    st7701_write_data(0x00);

    st7701_write_cmd(0xEB);
    st7701_write_data(0x00); 	
    st7701_write_data(0x00);
    st7701_write_data(0x40);
    st7701_write_data(0x40);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);  

    st7701_write_cmd(0xED);  
    st7701_write_data(0xFF); 
    st7701_write_data(0xFF);  
    st7701_write_data(0xFF); 	
    st7701_write_data(0xBA); 		
    st7701_write_data(0x0A); 	
    st7701_write_data(0xBF); 	
    st7701_write_data(0x45); 	
    st7701_write_data(0xFF); 
    st7701_write_data(0xFF);  
    st7701_write_data(0x54); 	
    st7701_write_data(0xFB); 	
    st7701_write_data(0xA0); 	
    st7701_write_data(0xAB); 	
    st7701_write_data(0xFF); 
    st7701_write_data(0xFF); 
    st7701_write_data(0xFF); 

    st7701_write_cmd(0xEF);
    st7701_write_data(0x10); 
    st7701_write_data(0x0D); 
    st7701_write_data(0x04); 
    st7701_write_data(0x08); 
    st7701_write_data(0x3F); 
    st7701_write_data(0x1F);

    st7701_write_cmd(0xFF);
    st7701_write_data(0x77);
    st7701_write_data(0x01);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x13);

    st7701_write_cmd(0xEF);
    st7701_write_data(0x08);

    st7701_write_cmd(0xFF);
    st7701_write_data(0x77);
    st7701_write_data(0x01);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);

    st7701_write_cmd(0x36);
    st7701_write_data(0x00);

    st7701_write_cmd(0x3A);    //0x70 RGB888, 0x60 RGB666, 0x50 RGB565
	st7701_write_data(0x60);

#else

    st7701_write_cmd(0xFF);
    st7701_write_data(0x77);
    st7701_write_data(0x01);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x10);

    st7701_write_cmd(0xC0);
    st7701_write_data(0x3B);
    st7701_write_data(0x00);

    st7701_write_cmd(0xC1);
    st7701_write_data(0x0B); // VBP
    st7701_write_data(0x02);

    st7701_write_cmd(0xC2);
    st7701_write_data(0x00);
    st7701_write_data(0x02);

    st7701_write_cmd(0xCC);
    st7701_write_data(0x10);

    st7701_write_cmd(0xCD);
    st7701_write_data(0x08);

    st7701_write_cmd(0xB0); // Positive Voltage Gamma Control
    st7701_write_data(0x02);
    st7701_write_data(0x13);
    st7701_write_data(0x1B);
    st7701_write_data(0x0D);
    st7701_write_data(0x10);
    st7701_write_data(0x05);
    st7701_write_data(0x08);
    st7701_write_data(0x07);
    st7701_write_data(0x07);
    st7701_write_data(0x24);
    st7701_write_data(0x04);
    st7701_write_data(0x11);
    st7701_write_data(0x0E);
    st7701_write_data(0x2C);
    st7701_write_data(0x33);
    st7701_write_data(0x1D);

    st7701_write_cmd(0xB1); // Negative Voltage Gamma Control
    st7701_write_data(0x05);
    st7701_write_data(0x13);
    st7701_write_data(0x1B);
    st7701_write_data(0x0D);
    st7701_write_data(0x11);
    st7701_write_data(0x05);
    st7701_write_data(0x08);
    st7701_write_data(0x07);
    st7701_write_data(0x07);
    st7701_write_data(0x24);
    st7701_write_data(0x04);
    st7701_write_data(0x11);
    st7701_write_data(0x0E);
    st7701_write_data(0x2C);
    st7701_write_data(0x33);
    st7701_write_data(0x1D);

    st7701_write_cmd(0xFF);
    st7701_write_data(0x77);
    st7701_write_data(0x01);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x11);

    st7701_write_cmd(0xB0);
    st7701_write_data(0x5d); // 5d

    st7701_write_cmd(0xB1);  // VCOM amplitude setting
    st7701_write_data(0x43); // 43

    st7701_write_cmd(0xB2);  // VGH Voltage setting
    st7701_write_data(0x81); // 12V

    st7701_write_cmd(0xB3);
    st7701_write_data(0x80);

    st7701_write_cmd(0xB5);  // VGL Voltage setting
    st7701_write_data(0x43); //-8.3V

    st7701_write_cmd(0xB7);
    st7701_write_data(0x85);

    st7701_write_cmd(0xB8);
    st7701_write_data(0x20);

    st7701_write_cmd(0xC1);
    st7701_write_data(0x78);

    st7701_write_cmd(0xC2);
    st7701_write_data(0x78);

    st7701_write_cmd(0xD0);
    st7701_write_data(0x88);

    st7701_write_cmd(0xE0);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x02);

    st7701_write_cmd(0xE1);
    st7701_write_data(0x03);
    st7701_write_data(0xA0);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x04);
    st7701_write_data(0xA0);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x20);
    st7701_write_data(0x20);

    st7701_write_cmd(0xE2);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);

    st7701_write_cmd(0xE3);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x11);
    st7701_write_data(0x00);

    st7701_write_cmd(0xE4);
    st7701_write_data(0x22);
    st7701_write_data(0x00);

    st7701_write_cmd(0xE5);
    st7701_write_data(0x05);
    st7701_write_data(0xEC);
    st7701_write_data(0xA0);
    st7701_write_data(0xA0);
    st7701_write_data(0x07);
    st7701_write_data(0xEE);
    st7701_write_data(0xA0);
    st7701_write_data(0xA0);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);

    st7701_write_cmd(0xE6);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x11);
    st7701_write_data(0x00);

    st7701_write_cmd(0xE7);
    st7701_write_data(0x22);
    st7701_write_data(0x00);

    st7701_write_cmd(0xE8);
    st7701_write_data(0x06);
    st7701_write_data(0xED);
    st7701_write_data(0xA0);
    st7701_write_data(0xA0);
    st7701_write_data(0x08);
    st7701_write_data(0xEF);
    st7701_write_data(0xA0);
    st7701_write_data(0xA0);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);

    st7701_write_cmd(0xEB);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x40);
    st7701_write_data(0x40);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);

    st7701_write_cmd(0xED);
    st7701_write_data(0xFF);
    st7701_write_data(0xFF);
    st7701_write_data(0xFF);
    st7701_write_data(0xBA);
    st7701_write_data(0x0A);
    st7701_write_data(0xBF);
    st7701_write_data(0x45);
    st7701_write_data(0xFF);
    st7701_write_data(0xFF);
    st7701_write_data(0x54);
    st7701_write_data(0xFB);
    st7701_write_data(0xA0);
    st7701_write_data(0xAB);
    st7701_write_data(0xFF);
    st7701_write_data(0xFF);
    st7701_write_data(0xFF);

    st7701_write_cmd(0xEF);
    st7701_write_data(0x10);
    st7701_write_data(0x0D);
    st7701_write_data(0x04);
    st7701_write_data(0x08);
    st7701_write_data(0x3F);
    st7701_write_data(0x1F);

    st7701_write_cmd(0xFF);
    st7701_write_data(0x77);
    st7701_write_data(0x01);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x13);

    st7701_write_cmd(0xEF);
    st7701_write_data(0x08);

    st7701_write_cmd(0xFF);
    st7701_write_data(0x77);
    st7701_write_data(0x01);
    st7701_write_data(0x00);
    st7701_write_data(0x00);
    st7701_write_data(0x00);

    st7701_write_cmd(0x36);
    st7701_write_data(0x00);

    st7701_write_cmd(0x3A);   //0x70 RGB888, 0x60 RGB666, 0x50 RGB565
	st7701_write_data(0x60);

    // st7701_write_cmd(0x21);   //Display Inversion On
#endif
    st7701_write_cmd(0x11);   //Sleep Out
    vTaskDelay(pdMS_TO_TICKS(100));

	st7701_write_cmd(0x29);   //Display On
    vTaskDelay(pdMS_TO_TICKS(50));
}





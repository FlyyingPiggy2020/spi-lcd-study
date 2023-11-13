/*
 * Copyright (c) 2023 by Moorgen Tech. Co, Ltd.
 * @FilePath     : st7789v.c
 * @Author       : lxf
 * @Date         : 2023-11-10 15:38:40
 * @LastEditors  : flyyingpiggy2020 154562451@qq.com
 * @LastEditTime : 2023-11-13 21:31:56
 * @Brief        : ST7789V驱动程序
 */

/*---------- includes ----------*/

#include "st7789v.h"
/*---------- macro ----------*/

/**
 * @brief st7789v引脚定义
 * @return {*}
 */
#define PIN_NUM_CLK  10
#define PIN_NUM_MISO -1
#define PIN_NUM_MOSI 11
#define PIN_NUM_CS   9
#define PIN_NUM_DC   -1

#define BUFFER_SIZE  1024
/*---------- type define ----------*/

/*---------- variable prototype ----------*/
/*---------- function prototype ----------*/
/*---------- variable ----------*/
/*---------- function ----------*/
/**
 * @brief st7789v发送命令
 * @param {uint8_t} cmd
 * @return {*}
 */
void st7789v_cmd(uint8_t cmd)
{
    spi_transaction_t trans = {
        .length = 8,
        .tx_data[0] = cmd,
    }
}

/**
 * @brief st7789v发送数据
 * @param {uint8_t} *data
 * @param {uint8_t} len
 * @return {*}
 */
void st7789v_data(uint8_t *data, uint8_t len)
{
    return;
}

void st7789v_task(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(3000));
#define TAG "spi_task"
    ESP_LOGI(TAG, "create spi task");

    spi_device_handle_t spi;
    spi_bus_config_t bus_config = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = BUFFER_SIZE,
    };
    spi_device_interface_config_t dev_config = {
        .clock_speed_hz = 10 * 1000 * 1000, // SPI clock frequency
        .mode = 0,                          // SPI mode 0
        .spics_io_num = PIN_NUM_CS,         // CS pin
        .queue_size = 1,                    // We want to be able to queue 1 transaction at a time
    };
    spi_transaction_t trans = {
        .length = BUFFER_SIZE * 8, // Total data length, in bits
        .rx_buffer = NULL,         // Receive buffer
        .tx_buffer = malloc(BUFFER_SIZE),
    };

    // Initialize SPI bus
    esp_err_t ret = spi_bus_initialize(SPI3_HOST, &bus_config, SPI_DMA_CH_AUTO);
    assert(ret == ESP_OK);

    // Add the SPI device to the bus
    ret = spi_bus_add_device(SPI3_HOST, &dev_config, &spi);
    assert(ret == ESP_OK);

    // Fill the transmit buffer with test data
    for (int i = 0; i < BUFFER_SIZE; i++) {
        ((uint8_t *)trans.tx_buffer)[i] = i;
    }

    // Perform SPI transfer using DMA
    ret = spi_device_transmit(spi, &trans);
    assert(ret == ESP_OK);

    // Cleanup
    free((void *)trans.tx_buffer);
    spi_bus_remove_device(spi);
    spi_bus_free(SPI3_HOST);
    while (1) {
        vTaskDelay(10);
    }
    ESP_LOGI(TAG, "delte spi task");
    vTaskDelete(NULL);
#undef TAG
}

void st7789v_init(void)
{
#define TAG "st7789v"
    spi_device_handle_t spi;
    spi_bus_config_t bus_config = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .max_transfer_sz = BUFFER_SIZE,
    };
    spi_device_interface_config_t dev_config = {
        .clock_speed_hz = 10 * 1000 * 1000,
        .mode = 0,
        .spics_io_num = PIN_NUM_CS,
        .queue_size = 10,
    };

    esp_err_t ret = spi_bus_initialize(SPI3_HOST, &bus_config, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "spi bus initialize error");
    }

    ret = spi_bus_add_device(SPI3_HOST, &dev_config, &spi);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "spi bus add device error");
    }
#undef TAG
}
/*---------- end of file ----------*/

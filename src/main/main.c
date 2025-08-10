#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_spi_bus.h"
#include "esp_log.h"
#include "lvgl.h"

#define LCD_HOST         SPI2_HOST
#define LCD_PIXEL_CLOCK_HZ (40 * 1000 * 1000) // 40MHz
#define PIN_NUM_MOSI    35
#define PIN_NUM_CLK     36
#define PIN_NUM_CS      34
#define PIN_NUM_DC      33
#define PIN_NUM_RST     38
#define PIN_NUM_BK_LIGHT 39

#define LCD_H_RES       172
#define LCD_V_RES       320

static const char *TAG = "main";

// LVGL flush callback (LVGL v9 style)
static void lvgl_flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    esp_lcd_panel_handle_t panel_handle = lv_display_get_user_data(disp);
    int x1 = area->x1;
    int y1 = area->y1;
    int w = area->x2 - area->x1 + 1;
    int h = area->y2 - area->y1 + 1;

    esp_lcd_panel_draw_bitmap(panel_handle, x1, y1, x1 + w, y1 + h, px_map);
    lv_display_flush_ready(disp);
}

void app_main(void)
{
    ESP_LOGI(TAG, "Initializing SPI bus");
    esp_lcd_spi_bus_handle_t spi_bus = NULL;

    spi_bus_config_t buscfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = -1,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = LCD_H_RES * 40 * sizeof(uint16_t)
    };

    ESP_ERROR_CHECK(esp_lcd_new_spi_bus(&buscfg, &spi_bus));

    ESP_LOGI(TAG, "Attach panel IO to SPI bus");
    esp_lcd_panel_io_spi_config_t io_config = {
        .cs_gpio_num = PIN_NUM_CS,
        .dc_gpio_num = PIN_NUM_DC,
        .spi_mode = 0,
        .pclk_hz = LCD_PIXEL_CLOCK_HZ,
        .trans_queue_depth = 10,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
    };

    esp_lcd_panel_io_handle_t io_handle = NULL;
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(spi_bus, &io_config, &io_handle));

    ESP_LOGI(TAG, "Install LCD panel driver");
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = PIN_NUM_RST,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = 16,
    };

    esp_lcd_panel_handle_t panel_handle = NULL;
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle));

    ESP_LOGI(TAG, "Reset and init panel");
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, true, false));

    // Backlight on
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << PIN_NUM_BK_LIGHT
    };
    gpio_config(&bk_gpio_config);
    gpio_set_level(PIN_NUM_BK_LIGHT, 1);

    ESP_LOGI(TAG, "Initialize LVGL");
    lv_init();

    // Allocate draw buffer
    size_t buf_size = LCD_H_RES * 40 * sizeof(lv_color_t);
    lv_color_t *buf1 = heap_caps_malloc(buf_size, MALLOC_CAP_DMA);
    assert(buf1);

    // Create LVGL display
    lv_display_t *disp = lv_display_create(LCD_H_RES, LCD_V_RES);
    lv_display_set_flush_cb(disp, lvgl_flush_cb);
    lv_display_set_user_data(disp, panel_handle);
    lv_display_set_buffers(disp, buf1, NULL, buf_size, LV_DISPLAY_RENDER_MODE_PARTIAL);

    // Create a label
    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello World");
    lv_obj_center(label);

    // LVGL tick + task handler loop
    while (1) {
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

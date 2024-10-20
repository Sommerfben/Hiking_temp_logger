#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/dma.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"
#include "pico/cyw43_arch.h"
#include "hardware/uart.h"
#include "lib\temperature\temp_shtc3.h"
#include <stdlib.h>
#include "lib\LCD\LCD_Driver.h"
#include "lib\LCD\GUI_Paint.h"
#include "rp2040_config.h"
#include "lib\lvgl-release-v9.2\lvgl.h"
#include "lib\lvgl-release-v9.2\examples\lv_examples.h"

int64_t alarm_callback(alarm_id_t id, void *user_data)
{
   // Put your timeout handler code in here
   return(0);
}

// Data will be copied from src to dst
const char src[] = "Hello, world! (from DMA)";
char       dst[count_of(src)];

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (LCD_WIDTH * LCD_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];


int main()
{
   init();
   
   LCD_Init();
   lv_init();
   lv_display_t * display = lv_display_create(LCD_WIDTH, LCD_HEIGHT);
   lv_display_set_antialiasing(display, true);
   
   /*Set a tick source so that LVGL will know how much time elapsed. */
   lv_tick_set_cb(time_us_32);
   lv_display_set_flush_cb(display, my_flush_cb);
   lv_display_set_buffers(display, draw_buf, NULL, sizeof(draw_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);
   shtc3 temp_sensor;
   uint16_t test_data = 0;

   printf("Hello, world!\n");
   float temp_data[2];
   temp_sensor.make_low_power_measurement_blocking(temp_data);
   printf("Temperature: %f\n", temp_data[0]);
   printf("Humidity: %f\n", temp_data[1]);
   
   /**
    * Basic example to create a "Hello world" label
    */
       /*Change the active screen's background color*/
   lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0), LV_PART_MAIN);

   /*Create a white label, set its text and align it to the center*/
   lv_obj_t * label = lv_label_create(lv_screen_active());
   lv_label_set_text(label, "Hello world");
   lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xFFFFFF), LV_PART_MAIN);
   lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
   
   /*Create an Arc*/
   lv_obj_t * arc = lv_arc_create(lv_screen_active());
   lv_obj_set_size(arc, 230, 230);
   lv_obj_set_style_arc_color(arc, lv_palette_main(LV_PALETTE_GREEN), LV_PART_INDICATOR);
   lv_obj_set_style_arc_color(arc, lv_palette_main(LV_PALETTE_DEEP_PURPLE), LV_PART_MAIN);
   lv_arc_set_rotation(arc, 180);
   lv_arc_set_range(arc, 0, 40);
   lv_arc_set_bg_angles(arc, 0, 360);

   lv_obj_remove_style(arc, NULL, LV_PART_KNOB);   /*Be sure the knob is not displayed*/
   lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);  /*To not allow adjusting by click*/
   lv_obj_center(arc);

   while (true)
   {
      lv_timer_handler(); /* let the GUI do its work */ 
      sleep_ms(100);
      lv_label_set_text_fmt(label, "Temp: %d", (int)temp_data[0]);
      lv_arc_set_value(arc, ((int)temp_data[0]));
      temp_sensor.make_low_power_measurement_blocking(temp_data);
      printf("Temperature: %f\n", temp_data[0]);
      printf("Humidity: %f\n", temp_data[1]);
   }
}


void my_flush_cb(lv_display_t * display, const lv_area_t * area, uint8_t * px_map)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one
     *`put_px` is just an example, it needs to be implemented by you.*/
    uint16_t * buf16 = (uint16_t *)px_map; /*Let's say it's a 16 bit (RGB565) display*/
    int32_t x, y;
    for(y = area->y1; y <= area->y2; y++) {
        for(x = area->x1; x <= area->x2; x++) {
            LCD_SetUWORD(x, y, *buf16);
            buf16++;
        }
    }
   // new_color = (color_p->blue << 11) | (color_p->red << 5) | (color_p->green >> 1);

    /* IMPORTANT!!!
     * Inform LVGL that you are ready with the flushing and buf is not used anymore*/
    lv_display_flush_ready(display);
}

// Converts a 16bit RGB565 color into the correct BRG565 color profile for this display
uint16_t convert_color(uint16_t color) 
{
    // Extract Red (bits 11-15), Green (bits 5-10), and Blue (bits 0-4)
    uint8_t red   = (color >> 11) & 0x1F;  // Extract 5 bits for Red
    uint8_t green = (color >> 5) & 0x3F;   // Extract 6 bits for Green
    uint8_t blue  = color & 0x1F;          // Extract 5 bits for Blue

    // Rearrange into the new format: Blue (5 bits), Red (6 bits), Green (5 bits)
    uint16_t new_color = (blue << 11) | (red << 5) | (green >> 1);

    return new_color;
}


bool init()
{
    stdio_init_all();

   // Initialise the Wi-Fi chip
   if (cyw43_arch_init())
   {
      printf("Wi-Fi init failed\n");
      return(-1);
   }

   // SPI initialisation. This example will use SPI at 1MHz.
   spi_init(SPI_PORT, SPI_BUS_SPEED_HZ);
   gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
   gpio_set_function(PIN_CS, GPIO_FUNC_SIO);
   gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
   gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

   // GPIO init
   gpio_init(DEV_CS_PIN);
   gpio_set_dir(DEV_CS_PIN, GPIO_OUT);

   Config_Init();
   LCD_Init();
   Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 0, BLACK);
   Paint_Clear(WHITE);

   // Chip select is active-low, so we'll initialise it to a driven-high state
   gpio_set_dir(PIN_CS, GPIO_OUT);
   gpio_put(PIN_CS, 1);
   // For more examples of SPI use see https://github.com/raspberrypi/pico-examples/tree/master/spi

   // I2C Initialisation. Using it at 400Khz.
   i2c_init(I2C_PORT, I2C_BUS_SPEED_HZ);

   gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
   gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
   gpio_pull_up(I2C_SDA);
   gpio_pull_up(I2C_SCL);
   // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

   // Get a free channel, panic() if there are none
   int chan = dma_claim_unused_channel(true);

   // 8 bit transfers. Both read and write address increment after each
   // transfer (each pointing to a location in src or dst respectively).
   // No DREQ is selected, so the DMA transfers as fast as it can.

   dma_channel_config c = dma_channel_get_default_config(chan);
   channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
   channel_config_set_read_increment(&c, true);
   channel_config_set_write_increment(&c, true);

   dma_channel_configure(
      chan,            // Channel to be configured
      &c,              // The configuration we just created
      dst,             // The initial write address
      src,             // The initial read address
      count_of(src),   // Number of transfers; in this case each is 1 byte.
      true             // Start immediately.
      );

   // We could choose to go and do something else whilst the DMA is doing its
   // thing. In this case the processor has nothing else to do, so we just
   // wait for the DMA to finish.
   dma_channel_wait_for_finish_blocking(chan);

   // The DMA has now copied our text from the transmit buffer (src) to the
   // receive buffer (dst), so we can print it out from there.
   puts(dst);

   // Timer example code - This example fires off the callback after 2000ms
   add_alarm_in_ms(2000, alarm_callback, NULL, false);
   // For more examples of timer use see https://github.com/raspberrypi/pico-examples/tree/master/timer

   printf("System Clock Frequency is %d Hz\n", clock_get_hz(clk_sys));
   printf("USB Clock Frequency is %d Hz\n", clock_get_hz(clk_usb));
   // For more examples of clocks use see https://github.com/raspberrypi/pico-examples/tree/master/clocks

   // Example to turn on the Pico W LED
   cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

   // Set up our UART
   uart_init(UART_ID, BAUD_RATE);
   // Set the TX and RX pins by using the function select on the GPIO
   // Set datasheet for more information on function select
   gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
   gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

   // Use some the various UART functions to send out data
   // In a default system, printf will also output via the default UART

   // Send out a string, with CR/LF conversions
   uart_puts(UART_ID, " Hello, UART!\n");

    return 1;
}
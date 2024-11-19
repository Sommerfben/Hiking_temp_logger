#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/dma.h"
#include "hardware/timer.h"
#include "hardware/irq.h"
#include "hardware/clocks.h"
#include "pico/cyw43_arch.h"
#include "hardware/uart.h"
#include "lib\temperature\temp_shtc3.h"
#include <stdlib.h>
#include "lib\LCD\LCD_Driver.h"
#include "lib\LCD\GUI_Paint.h"
#include "rp2040_config.h"
shtc3 temp_sensor;
float temp_data[2];

bool timer_callback(repeating_timer_t *rt)
{
    printf("Fired Interrupt!\n");
     //temp_sensor.make_low_power_measurement_blocking(temp_data);
    printf("Temperature: %f\n", temp_data[0]);
    printf("Humidity: %f\n", temp_data[1]);

    return(true);
}

// Data will be copied from src to dst
const char src[] = "Hello, world! (from DMA)";
char       dst[count_of(src)];

uint8_t hz = 1;

int main()
{
    init();
    uint16_t test_data = 0;

    repeating_timer_t sampleTimer;
    add_repeating_timer_us(-1000000 / hz, timer_callback, NULL, &sampleTimer);

    printf("Hello, world!\n");
    
    while (true)
    {
     // RRRRR GGGGGG BBBBB: Conventional color mapping
     // BBBBB RRRRRR GGGGG: Mapping for this display module for some reason
     Paint_Clear(convert_color(GREEN)); //g
     sleep_ms(1000);
     temp_sensor.make_low_power_measurement_blocking(temp_data);
     sleep_ms(1000);
    }
}

// Converts a 16bit RGB565 color into the correct BRG565 color profile for this display
uint16_t convert_color(uint16_t color) 
{
     // Extract Red (bits 11-15), Green (bits 5-10), and Blue (bits 0-4)
     uint8_t red    = (color >> 11) & 0x1F;  // Extract 5 bits for Red
     uint8_t green = (color >> 5) & 0x3F;    // Extract 6 bits for Green
     uint8_t blue  = color & 0x1F;            // Extract 5 bits for Blue

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
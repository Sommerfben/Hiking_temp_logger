#pragma once

bool init();

// SPI Defines
// Use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT            spi0
#define SPI_BUS_SPEED_HZ    1 * 1000 * 1000
#define PIN_MISO            16
#define PIN_CS              10
#define PIN_SCK             18
#define PIN_MOSI            19

// I2C defines
// This will use I2C0 on GPIO12 (SDA) and GPIO13 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT            i2c0
#define I2C_BUS_SPEED_HZ    400 * 1000
#define I2C_SDA             12
#define I2C_SCL             13

// UART defines
// By default the stdout UART is `uart0`, so use the second one
#define UART_ID      uart1
#define BAUD_RATE    115200

// Use pins 4 and 5 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN    4
#define UART_RX_PIN    5

#include <stdio.h>
#include <lib/temperature/temp_shtc3.h>
#include <cstdint>
#include "hardware/i2c.h"
#include "rp2040_config.h"

/**
 * Reads and converts the results of the most recent measurement command
 *
 * @param   data                The float array to populate with results
 * @param   temperature_first   Must be true if the last command was a TFIRST command, false otherwise
 *
 * @return  true if successful
 */
uint16_t shtc3::read_data(float *data, bool temperature_first)
{
   uint8_t regData[6];
   uint8_t status;

   status = i2c_read_blocking(I2C_PORT, SHTC3_DEFAULT_ADDR, regData, 6, false);

   float convertedTemperatureReading;
   float convertedHumidityReading;

   // Extract data from read buffer and build the raw measurement values
   if (temperature_first)
   {
      convertedTemperatureReading = ((regData[0] << 8) | regData[1]);
      convertedHumidityReading    = ((regData[3] << 8) | regData[4]);
   }
   else
   {
      convertedTemperatureReading = ((regData[3] << 8) | regData[4]);
      convertedHumidityReading    = ((regData[0] << 8) | regData[1]);
   }

   // Convert raw measurement values to decimal
   convertedTemperatureReading = convertedTemperatureReading * 175;
   convertedTemperatureReading = convertedTemperatureReading / 65536;
   convertedTemperatureReading = convertedTemperatureReading - 45;

   convertedHumidityReading = convertedHumidityReading / 65536;
   convertedHumidityReading = convertedHumidityReading * 100;

   data[0] = convertedTemperatureReading;
   data[1] = convertedHumidityReading;
   return(1);
}

/**
 * Performs an I2C write to send a command to the device
 *
 * @param   command   The 16-bit command ID to send.
 * @return  true if successful
 */
bool shtc3::write_command(uint16_t command)
{
   uint8_t commandData[2];

   commandData[0] = command >> 8;
   commandData[1] = command & 0xFF;
   return(i2c_write_blocking(I2C_PORT, SHTC3_DEFAULT_ADDR, commandData, 2, false));
}

/**
 * Perform a low power measurement of temperature and humidity. 
 * Populates a data array with [Temperature Reading, Humidity Reading]
 *
 * @param   data   The float array to populate with results
 * @return  true if successful
 */
bool shtc3::make_low_power_measurement_blocking(float *data)
{
   write_command(SHTC3_WAKEUP);
   sleep_us(SHTC3_WAKE_TIME_US);
   write_command(SHTC3_LOWPOW_MEAS_TFIRST);
   sleep_us(SHTC3_LOW_POWER_MEAS_MAX_TIME_US);
   read_data(data, true);
   write_command(SHTC3_SLEEP);
   return(1);
}

/**
 * Perform a high power measurement of temperature and humidity. 
 * Populates a data array with [Temperature Reading, Humidity Reading]
 *
 * @param   data   The float array to populate with results
 * @return  true if successful
 */
bool shtc3::make_high_power_measurement_blocking(float *data)
{
   write_command(SHTC3_WAKEUP);
   sleep_us(SHTC3_WAKE_TIME_US);
   write_command(SHTC3_NORMAL_MEAS_TFIRST);
   sleep_ms(SHTC3_NORMAL_POWER_MEAS_MAX_TIME_MS);
   read_data(data, true);
   write_command(SHTC3_SLEEP);
   return(1);
}

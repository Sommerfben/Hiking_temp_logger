#pragma once
#include <cstdint>

#define SHTC3_DEFAULT_ADDR                     0x70 // SHTC3 I2C Address

#define SHTC3_NORMAL_POWER_MEAS_MAX_TIME_MS    12   // Maximum time for a normal power measurement to complete
#define SHTC3_LOW_POWER_MEAS_MAX_TIME_US       800  // Maximum time for a low power measurement to complete
#define SHTC3_WAKE_TIME_US                     250

#define SHTC3_NORMAL_MEAS_TFIRST               0x7866 // Normal measurement, temperature first with Clock Stretch disabled
#define SHTC3_NORMAL_MEAS_HFIRST               0x58E0 // Normal measurement, humidity first with Clock Stretch disabled
#define SHTC3_NORMAL_MEAS_TFIRST_STRETCH       0x7CA2 // Normal measurement temperature first with Clock Stretch Enabled
#define SHTC3_NORMAL_MEAS_HFIRST_STRETCH       0x5C24 // Normal measurement, humidity first with Clock Stretch Enabled
#define SHTC3_LOWPOW_MEAS_TFIRST               0x609C // Low power measurement, temperature first with Clock Stretch disabled
#define SHTC3_LOWPOW_MEAS_HFIRST               0x401A // Low power measurement, humidity first with Clock Stretch disabled
#define SHTC3_LOWPOW_MEAS_TFIRST_STRETCH       0x6458 // Low power measurement, temperature first with Clock Stretch Enabled
#define SHTC3_LOWPOW_MEAS_HFIRST_STRETCH       0x44DE // Low power measurement, humidity first with Clock Stretch Enabled
#define SHTC3_READID                           0xEFC8 // Read out ID Register
#define SHTC3_SOFTRESET                        0x805D // Soft Reset
#define SHTC3_SLEEP                            0xB098 // Enter sleep mode
#define SHTC3_WAKEUP                           0x3517 // Wakeup mode


class shtc3
{
private:
   uint16_t read_data(float *data, bool temperature_first);
   bool write_command(uint16_t command);

public:
   bool make_low_power_measurement_blocking(float *data);
   bool make_high_power_measurement_blocking(float *data);
};

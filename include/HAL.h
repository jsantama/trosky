#ifndef HAL_H
#define HAL_H

// [SESSION PROTOCOL 2.114 - 4.3] Hardware Abstraction (HAL)
// Logic must be decoupled from specific pinouts.

// Node: Bathroom (ESP32-S3 + TFT + BMP280)
namespace HAL {
namespace Bathroom {
constexpr int PIN_SOS = 4;
constexpr int PIN_VOICE = 5;

// Display (ST7789)
constexpr int PIN_TFT_BL = 38;

// I2C
constexpr int I2C_SDA = 8;
constexpr int I2C_SCL = 9;
constexpr int BMP280_ADDR = 0x6B;
} // namespace Bathroom

namespace LivingRoom {
constexpr int I2S_WS = 42;
constexpr int I2S_SD = 41;
constexpr int I2S_SCK = 1;
} // namespace LivingRoom
} // namespace HAL

#endif

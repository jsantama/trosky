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

// Camera OV2640 (Standard S3-CAM)
constexpr int CAM_D0 = 11;
constexpr int CAM_D1 = 9;
constexpr int CAM_D2 = 8;
constexpr int CAM_D3 = 10;
constexpr int CAM_D4 = 12;
constexpr int CAM_D5 = 18;
constexpr int CAM_D6 = 17;
constexpr int CAM_D7 = 16;
constexpr int CAM_XCLK = 15;
constexpr int CAM_PCLK = 13;
constexpr int CAM_VSYNC = 6;
constexpr int CAM_HREF = 7;
constexpr int CAM_SDA = 4;
constexpr int CAM_SCL = 5;
constexpr int CAM_PWDN = -1;
constexpr int CAM_RESET = -1;
} // namespace LivingRoom
} // namespace HAL

#endif

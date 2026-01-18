#include "app/modules/bathroom/BathroomNode.h"
#include "HAL.h"
#include "config.h"
#include <Wire.h>
#include <time.h>

BathroomNode::BathroomNode()
    : AppState(nullptr), EventBus(nullptr), sensorTool(nullptr),
      inputTool(nullptr), notificationTool(nullptr), ui(nullptr) {}

void BathroomNode::init() {
  // Initialize I2C [SESSION PROTOCOL 4.3 - HAL Mapping]
  Wire.begin(HAL::Bathroom::I2C_SDA, HAL::Bathroom::I2C_SCL);

  // Initialize SNTP
  configTime(TIMEZONE_OFFSET, 0, NTP_SERVER_PRIMARY, NTP_SERVER_SECONDARY);

  // Tools initialization
  sensorTool = new SensorTool(AppState, EventBus);
  sensorTool->init(HAL::Bathroom::BMP280_ADDR);

  inputTool = new InputTool(AppState, EventBus);
  inputTool->init(HAL::Bathroom::PIN_SOS, HAL::Bathroom::PIN_VOICE);

  // [New] WhatsApp Service must be registered first in ServiceLocator
  auto *whatsApp =
      ServiceLocator::instance().get<WhatsAppService>("WhatsAppService");
  notificationTool = new NotificationTool(EventBus, whatsApp);
  notificationTool->init();

  // UI initialization
  ui = new BathroomUI(AppState);
  ui->init();
}

void BathroomNode::loop() {
  if (sensorTool)
    sensorTool->update();
  if (inputTool)
    inputTool->update();
  if (ui)
    ui->update();
}

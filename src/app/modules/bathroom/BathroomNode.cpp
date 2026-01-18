#include "app/modules/bathroom/BathroomNode.h"
#include "HAL.h"
#include "config.h"
#include "engine/ServiceLocator.h"
#include <Wire.h>
#include <time.h>

BathroomNode::BathroomNode()
    : AppState(nullptr), eventBus(nullptr), sensorTool(nullptr),
      inputTool(nullptr), notificationTool(nullptr), ui(nullptr) {}

BathroomNode::~BathroomNode() {
  // [SESSION PROTOCOL 2.114 - Memory Management]
  delete sensorTool;
  delete inputTool;
  delete notificationTool;
  delete ui;
}

void BathroomNode::init() {
  Serial.println("[NODE] Initializing BathroomNode...");

  // 1. UI initialization (Prioritize to turn on backlight)
  Serial.println("[NODE] Initializing UI...");
  ui = new BathroomUI(AppState);
  ui->init();

  // 2. Initialize I2C [SESSION PROTOCOL 4.3 - HAL Mapping]
  Serial.println("[NODE] Initializing I2C...");
  Wire.begin(HAL::Bathroom::I2C_SDA, HAL::Bathroom::I2C_SCL);

  // 3. Initialize SNTP
  Serial.println("[NODE] ConfigTime SNTP...");
  configTime(TIMEZONE_OFFSET, 0, NTP_SERVER_PRIMARY, NTP_SERVER_SECONDARY);

  // 4. Tools initialization
  Serial.println("[NODE] Initializing SensorTool...");
  sensorTool = new SensorTool(AppState, eventBus);
  if (!sensorTool->init(HAL::Bathroom::BMP280_ADDR)) {
    Serial.println(
        "[WARN] SensorTool init failed (Check I2C address or wiring)");
  }

  Serial.println("[NODE] Initializing InputTool...");
  inputTool = new InputTool(AppState, eventBus);
  inputTool->init(HAL::Bathroom::PIN_SOS, HAL::Bathroom::PIN_VOICE);

  // 5. Service validation
  Serial.println("[NODE] Validating Services...");
  auto *whatsApp =
      ServiceLocator::instance().get<WhatsAppService>("WhatsAppService");
  if (!whatsApp) {
    Serial.println("[ERROR] WhatsAppService not registered");
    return;
  }

  notificationTool = new NotificationTool(eventBus, whatsApp);
  notificationTool->init();

  Serial.println("[NODE] BathroomNode Initialized Successfully");
}

void BathroomNode::loop() {
  if (sensorTool)
    sensorTool->update();
  if (inputTool)
    inputTool->update();
  if (ui)
    ui->update();
}

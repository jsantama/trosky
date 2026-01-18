#include "app/modules/bathroom/BathroomNode.h"
#include "HAL.h"

BathroomNode::BathroomNode()
    : AppState(nullptr), EventBus(nullptr), sensorTool(nullptr),
      inputTool(nullptr), notificationTool(nullptr), ui(nullptr) {}

void BathroomNode::init() {
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

#include "engine/tools/SensorTool.h"
#include <Arduino.h>

SensorTool::SensorTool(IAppState *state, EventBus *bus)
    : appState(state), eventBus(bus), lastUpdate(0) {}

bool SensorTool::init(uint8_t addr) {
  if (!bmp.begin(addr)) {
    Serial.println("SensorTool: Failed to init BMP280");
    return false;
  }
  return true;
}

void SensorTool::update() {
  unsigned long now = millis();
  if (now - lastUpdate >= UPDATE_INTERVAL) {
    lastUpdate = now;

    float temp = bmp.readTemperature();
    float press = bmp.readPressure();

    if (appState) {
      appState->setTemperature(temp);
      appState->setPressure(press);
    }

    if (eventBus) {
      // Assuming EventType is defined in EventBus.h
      eventBus->publish(EventType::SENSOR_DATA_UPDATED);
    }
  }
}

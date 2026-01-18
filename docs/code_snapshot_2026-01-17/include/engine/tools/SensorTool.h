#ifndef SENSOR_TOOL_H
#define SENSOR_TOOL_H

#include "engine/EventBus.h"
#include "engine/IAppState.h"
#include <Adafruit_BMP280.h>

/**
 * [SESSION PROTOCOL 2.114 - 3.3] Smart Tools
 * Handles sensor logic and modifies AppState.
 */
class SensorTool {
public:
  SensorTool(IAppState *state, EventBus *bus);
  bool init(uint8_t addr);
  void update();

private:
  IAppState *appState;
  EventBus *eventBus;
  Adafruit_BMP280 bmp;
  unsigned long lastUpdate;
  static constexpr unsigned long UPDATE_INTERVAL = 2000;
};

#endif

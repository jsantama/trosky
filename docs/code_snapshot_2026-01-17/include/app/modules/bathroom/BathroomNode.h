#ifndef BATHROOM_NODE_H
#define BATHROOM_NODE_H

#include "app/modules/bathroom/BathroomUI.h"
#include "engine/EventBus.h"
#include "engine/IAppState.h"
#include "engine/tools/InputTool.h"
#include "engine/tools/NotificationTool.h"
#include "engine/tools/SensorTool.h"

/**
 * [SESSION PROTOCOL 2.114 - 3.4] Module Architecture
 * Orchestrates tools and UI.
 */
class BathroomNode {
public:
  BathroomNode();
  ~BathroomNode(); // [SESSION PROTOCOL 2.114 - Memory Management]
  void init();
  void loop();

  // Injected Services
  IAppState *AppState;
  EventBus *EventBus;

private:
  SensorTool *sensorTool;
  InputTool *inputTool;
  NotificationTool *notificationTool;
  BathroomUI *ui;
};

#endif

#ifndef LIVING_ROOM_NODE_H
#define LIVING_ROOM_NODE_H

#include "engine/EventBus.h"
#include "engine/IAppState.h"
#include "engine/tools/NotificationTool.h"
#include "services/WhatsAppService.h"

class LivingRoomNode {
public:
  LivingRoomNode();
  ~LivingRoomNode(); // [SESSION PROTOCOL 2.114 - Memory Management]
  void init();
  void loop();

  // Dependencies  // Injected Services
  EventBus *eventBus; // Renamed from EventBus to avoid naming conflict
  IAppState *AppState;

private:
  void setupI2S();
  bool setupCamera();
  bool detectWakeWord();
  String captureIntent();
  void processTaxiRequest();
  void processRecipeRequest();

  WhatsAppService *whatsApp;
  NotificationTool *notificationTool;
};

#endif

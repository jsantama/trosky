#ifndef LIVING_ROOM_NODE_H
#define LIVING_ROOM_NODE_H

#include "engine/EventBus.h"
#include "engine/IAppState.h"
#include "services/WhatsAppService.h"

class LivingRoomNode {
public:
  LivingRoomNode();
  void init();
  void loop();

  // Dependencies (Injected)
  EventBus *EventBus;
  IAppState *AppState;

private:
  void setupI2S();
  bool detectWakeWord();
  String captureIntent();
  void processTaxiRequest();
  void processRecipeRequest();

  WhatsAppService *whatsApp;
};

#endif

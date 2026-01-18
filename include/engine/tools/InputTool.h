#ifndef INPUT_TOOL_H
#define INPUT_TOOL_H

#include "engine/EventBus.h"
#include "engine/IAppState.h"

/**
 * [SESSION PROTOCOL 2.114 - 3.3] Smart Tools
 * Handles button logic and SOS triggers.
 */
class InputTool {
public:
  InputTool(IAppState *state, EventBus *bus);
  void init(int pinSOS, int pinVoice);
  void update();

private:
  IAppState *appState;
  EventBus *eventBus;

  int pinSOS;
  int pinVoice;

  unsigned long sosStartTime;
  bool lastVoiceState;

  static constexpr unsigned long SOS_THRESHOLD = 1000;
};

#endif

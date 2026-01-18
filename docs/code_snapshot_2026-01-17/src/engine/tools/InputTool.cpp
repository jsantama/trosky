#include "engine/tools/InputTool.h"
#include <Arduino.h>

InputTool::InputTool(IAppState *state, EventBus *bus)
    : appState(state), eventBus(bus), pinSOS(-1), pinVoice(-1), sosStartTime(0),
      lastVoiceState(false) {}

void InputTool::init(int pSOS, int pVoice) {
  pinSOS = pSOS;
  pinVoice = pVoice;
  pinMode(pinSOS, INPUT_PULLUP);
  pinMode(pinVoice, INPUT_PULLUP);
}

void InputTool::update() {
  unsigned long now = millis();

  // SOS Logic (Active Low)
  if (digitalRead(pinSOS) == LOW) {
    if (sosStartTime == 0)
      sosStartTime = now;
    if (now - sosStartTime > SOS_THRESHOLD) {
      if (appState && !appState->isSOS()) {
        appState->setSOS(true);
        if (eventBus)
          eventBus->publish(EventType::SOS_TRIGGERED);
      }
    }
  } else {
    sosStartTime = 0;
  }

  // Voice / Listening Logic
  bool currentVoiceState = (digitalRead(pinVoice) == LOW);
  if (currentVoiceState != lastVoiceState) {
    lastVoiceState = currentVoiceState;
    if (appState)
      appState->setListening(currentVoiceState);
    if (eventBus) {
      eventBus->publish(currentVoiceState ? EventType::VOICE_START_LISTENING
                                          : EventType::VOICE_STOP_LISTENING);
    }
  }
}

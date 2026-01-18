#ifndef BATHROOM_UI_H
#define BATHROOM_UI_H

#include "engine/IAppState.h"
#include <Arduino.h>
#include <TFT_eSPI.h>

/**
 * [SESSION PROTOCOL 2.114 - 3.3] Dumb Canvas
 * Only renders state, no business logic.
 */
class BathroomUI {
public:
  BathroomUI(IAppState *state);
  void init();
  void update();

private:
  IAppState *appState;
  TFT_eSPI tft;

  unsigned long lastAnimTime;
  int tailAngle;
  bool tailUp;

  // [State Tracking for Anti-Flicker]
  float lastTemp;
  bool lastListening;
  bool lastSOS;
  int lastHour;
  int lastMin;
  bool lastColonVisible;

  void drawBackground(float temp, bool force);
  void drawTrosky(int x, int y, bool wag);
  void drawSOS();
  void drawDayPhase(int x, int y, int hour);
  void drawTextWithOutline(int x, int y, const char *text, uint16_t color,
                           uint16_t outlineColor, uint8_t size = 1);
  uint16_t interpolateColor(float temp);
};

#endif

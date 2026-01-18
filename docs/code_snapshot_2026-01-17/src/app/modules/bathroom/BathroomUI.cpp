#include "app/modules/bathroom/BathroomUI.h"
#include "HAL.h"
#include <time.h>

BathroomUI::BathroomUI(IAppState *state)
    : appState(state), tailAngle(0), tailUp(true), lastTemp(-100.0f),
      lastListening(false), lastSOS(false), lastHour(-1), lastMin(-1),
      lastColonVisible(true) {
  lastAnimTime = millis();
}

void BathroomUI::init() {
  pinMode(HAL::Bathroom::PIN_TFT_BL, OUTPUT);
  digitalWrite(HAL::Bathroom::PIN_TFT_BL, HIGH);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
}

void BathroomUI::update() {
  if (!appState)
    return;

  bool currentSOS = appState->isSOS();

  // SOS Blink / State change
  if (currentSOS) {
    drawSOS();
    lastSOS = true;
    return;
  } else if (lastSOS) {
    // Clear SOS screen when it ends
    tft.fillScreen(TFT_BLACK);
    lastSOS = false;
  }

  float temp = appState->getTemperature();
  bool listening = appState->isListening();

  // Get current time
  time_t now;
  struct tm timeinfo;
  bool timeValid = false;
  if (time(&now) > 100000) { // Check if SNTP has synced
    localtime_r(&now, &timeinfo);
    timeValid = true;
  }

  // Force redraw if everything is new or temp changed significantly
  bool forceRedraw = (abs(temp - lastTemp) > 0.5f);
  drawBackground(temp, forceRedraw);

  // Draw Day Phase (Sun/Moon)
  if (timeValid && (timeinfo.tm_hour != lastHour || forceRedraw)) {
    drawDayPhase(200, 20, timeinfo.tm_hour);
    lastHour = timeinfo.tm_hour;
  }

  // Draw Time with blinking colon
  static unsigned long lastColonBlink = 0;
  bool colonVisible = (millis() / 500) % 2 == 0;

  if (timeValid) {
    if (timeinfo.tm_min != lastMin || colonVisible != lastColonVisible ||
        forceRedraw) {
      char timeBuf[16];
      snprintf(timeBuf, sizeof(timeBuf), "%02d%s%02d", timeinfo.tm_hour,
               colonVisible ? ":" : " ", timeinfo.tm_min);
      drawTextWithOutline(10, 10, timeBuf, TFT_WHITE, TFT_BLACK, 2);
      lastMin = timeinfo.tm_min;
      lastColonVisible = colonVisible;
    }
  } else {
    drawTextWithOutline(10, 10, "--:--", TFT_WHITE, TFT_BLACK, 2);
  }

  // Draw Temperature
  if (forceRedraw) {
    char tempBuf[16];
    snprintf(tempBuf, sizeof(tempBuf), "%.1f C", temp);
    drawTextWithOutline(10, 60, tempBuf, TFT_YELLOW, TFT_BLACK, 2);
    lastTemp = temp;
  }

  // Draw Trosky
  drawTrosky(160, 80, listening);
}

void BathroomUI::drawBackground(float temp, bool force) {
  if (!force)
    return;
  uint16_t bgColor = interpolateColor(temp);
  tft.fillRect(0, 0, 240, 135, bgColor);

  // [Pulse Effect] if listening
  if (appState->isListening()) {
    tft.drawRect(0, 0, 240, 135, TFT_WHITE);
    tft.drawRect(1, 1, 238, 133, TFT_WHITE);
  }
}

uint16_t BathroomUI::interpolateColor(float temp) {
  // 20C -> Blue (0,0,255), 30C -> Orange (255,165,0)
  float factor = (temp - 20.0f) / 10.0f;
  if (factor < 0)
    factor = 0;
  if (factor > 1)
    factor = 1;

  byte r = (byte)(0 + (255 - 0) * factor);
  byte g = (byte)(0 + (165 - 0) * factor);
  byte b = (byte)(255 + (0 - 255) * factor);

  return tft.color565(r, g, b);
}

void BathroomUI::drawDayPhase(int x, int y, int hour) {
  tft.fillRect(x - 15, y - 15, 30, 30,
               interpolateColor(appState->getTemperature())); // Clear area
  if (hour >= 6 && hour < 18) {
    // Sun
    tft.fillCircle(x, y, 10, TFT_YELLOW);
    for (int i = 0; i < 8; i++) {
      float ang = i * 0.785f;
      tft.drawLine(x + cos(ang) * 12, y + sin(ang) * 12, x + cos(ang) * 16,
                   y + sin(ang) * 16, TFT_YELLOW);
    }
  } else {
    // Moon
    tft.fillCircle(x, y, 10, TFT_WHITE);
    tft.fillCircle(x + 4, y - 4, 10,
                   interpolateColor(appState->getTemperature()));
  }
}

void BathroomUI::drawTextWithOutline(int x, int y, const char *text,
                                     uint16_t color, uint16_t outlineColor,
                                     uint8_t size) {
  tft.setTextSize(size);
  tft.setTextColor(outlineColor);
  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      if (dx == 0 && dy == 0)
        continue;
      tft.setCursor(x + dx, y + dy);
      tft.print(text);
    }
  }

  tft.setTextColor(color);
  tft.setCursor(x, y);
  tft.print(text);
}

void BathroomUI::drawTrosky(int x, int y, bool wag) {
  static bool lastWag = false;
  if (wag != lastWag) {
    // Clear tail area if state changed
    tft.fillRect(x - 15, y, 15, 30,
                 interpolateColor(appState->getTemperature()));
    lastWag = wag;
  }

  // Body
  tft.fillRoundRect(x, y, 40, 30, 5, tft.color565(139, 69, 19));
  // Head
  tft.fillCircle(x + 45, y + 5, 12, tft.color565(139, 69, 19));
  // Eye
  tft.fillCircle(x + 48, y + 2, 2, TFT_BLACK);

  if (wag) {
    if (millis() - lastAnimTime > 150) {
      // Clear old tail
      tft.drawLine(x, y + 15, x - 10, y + 15 + tailAngle,
                   interpolateColor(appState->getTemperature()));
      lastAnimTime = millis();
      tailAngle = tailUp ? 8 : -8;
      tailUp = !tailUp;
    }
  } else {
    tailAngle = 0;
  }
  // Draw current tail
  tft.drawLine(x, y + 15, x - 10, y + 15 + tailAngle,
               tft.color565(139, 69, 19));
}

void BathroomUI::drawSOS() {
  static bool blink = false;
  static unsigned long lastBlink = 0;

  if (millis() - lastBlink > 500) {
    lastBlink = millis();
    blink = !blink;
    uint16_t color = blink ? TFT_RED : TFT_BLACK;
    tft.fillScreen(color);

    if (blink) {
      tft.fillCircle(100, 60, 20, TFT_WHITE);
      tft.fillCircle(140, 60, 20, TFT_WHITE);
      tft.fillTriangle(80, 70, 160, 70, 120, 110, TFT_WHITE);
      drawTextWithOutline(40, 20, "SOS EMERGENCIA", TFT_WHITE, TFT_BLACK, 2);
    }
  }
}

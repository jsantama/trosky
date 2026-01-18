#include "app/modules/bathroom/BathroomUI.h"
#include "HAL.h"

BathroomUI::BathroomUI(IAppState *state)
    : appState(state), tailAngle(0), tailUp(true) {
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

  if (appState->isSOS()) {
    drawSOS();
    return;
  }

  float temp = appState->getTemperature();
  bool listening = appState->isListening();

  drawBackground(temp);

  // Draw Time (Static for now)
  drawTextWithOutline(10, 10, "12:34", TFT_WHITE, TFT_BLACK);

  // Draw Temperature
  char tempBuf[16];
  snprintf(tempBuf, sizeof(tempBuf), "%.1f C", temp);
  drawTextWithOutline(10, 50, tempBuf, TFT_WHITE, TFT_BLACK);

  // Draw Trosky
  drawTrosky(180, 80, listening);
}

void BathroomUI::drawBackground(float temp) {
  uint16_t bgColor = interpolateColor(temp);
  tft.fillRect(0, 0, 240, 135, bgColor);
}

uint16_t BathroomUI::interpolateColor(float temp) {
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

void BathroomUI::drawTextWithOutline(int x, int y, const char *text,
                                     uint16_t color, uint16_t outlineColor) {
  tft.setTextColor(outlineColor);
  tft.setCursor(x - 1, y);
  tft.print(text);
  tft.setCursor(x + 1, y);
  tft.print(text);
  tft.setCursor(x, y - 1);
  tft.print(text);
  tft.setCursor(x, y + 1);
  tft.print(text);

  tft.setTextColor(color);
  tft.setCursor(x, y);
  tft.print(text);
}

void BathroomUI::drawTrosky(int x, int y, bool wag) {
  tft.fillRoundRect(x, y, 40, 30, 5, tft.color565(139, 69, 19));
  tft.fillCircle(x + 45, y + 5, 12, tft.color565(139, 69, 19));
  tft.fillCircle(x + 48, y + 2, 2, TFT_BLACK);

  if (wag) {
    if (millis() - lastAnimTime > 100) {
      lastAnimTime = millis();
      tailAngle = tailUp ? 10 : -10;
      tailUp = !tailUp;
    }
  } else {
    tailAngle = 0;
  }
  tft.drawLine(x, y + 15, x - 10, y + 15 + tailAngle,
               tft.color565(139, 69, 19));
}

void BathroomUI::drawSOS() {
  static bool blink = false;
  static unsigned long lastBlink = 0;

  if (millis() - lastBlink > 500) {
    lastBlink = millis();
    blink = !blink;
  }

  uint16_t color = blink ? TFT_RED : TFT_BLACK;
  tft.fillScreen(color);

  if (blink) {
    tft.fillCircle(100, 60, 20, TFT_WHITE);
    tft.fillCircle(140, 60, 20, TFT_WHITE);
    tft.fillTriangle(80, 70, 160, 70, 120, 110, TFT_WHITE);
    drawTextWithOutline(80, 20, "SOS EMERGENICA", TFT_WHITE, TFT_BLACK);
  }
}

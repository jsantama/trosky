#include "app/modules/bathroom/BathroomUI.h"
#include "HAL.h"
#include <time.h>

BathroomUI::BathroomUI(IAppState *state)
    : appState(state), tft(HAL::Bathroom::PIN_TFT_CS, HAL::Bathroom::PIN_TFT_DC,
                           HAL::Bathroom::PIN_TFT_RST),
      tailAngle(0), tailUp(true), lastTemp(-100.0f), lastListening(false),
      lastSOS(false), lastHour(-1), lastMin(-1), lastColonVisible(true) {
  lastAnimTime = millis();
}

void BathroomUI::init() {
  Serial.println("[UI] Setting Backlight Pin...");
  pinMode(HAL::Bathroom::PIN_TFT_BL, OUTPUT);
  digitalWrite(HAL::Bathroom::PIN_TFT_BL, HIGH);

  Serial.println("[UI] TFT Init (Adafruit)...");
  tft.init(135, 240); // Init ST7789 240x135
  tft.setRotation(1);
  // Try inversion - typical for some ST7789 panels
  tft.invertDisplay(true);
  tft.fillScreen(ST77XX_BLACK);
  Serial.println("[UI] Init Config Complete");
}

void BathroomUI::update() {
  if (!appState)
    return;

  // SOS Logic
  if (appState->isSOS()) {
    drawSOS();
    lastSOS = true;
    return;
  } else if (lastSOS) {
    tft.fillScreen(ST77XX_BLACK);
    lastSOS = false;
    lastTemp = -100.0f;
  }

  float temp = appState->getTemperature();
  bool listening = appState->isListening();

  // DEBUG
  // Serial.printf("[UI] Temp: %.2f Listening: %d\n", temp, listening);

  // 1. Calculate background color based on Temp
  // 1. Calculate background color based on Temp & Time
  int currentHour = 12; // Default day
  time_t now;
  struct tm timeinfo;
  bool timeValid = (time(&now) > 100000);
  if (timeValid) {
    localtime_r(&now, &timeinfo);
    currentHour = timeinfo.tm_hour;
  }

  uint16_t bgColor = interpolateColor(temp, currentHour);

  // debug serials for time and temp
  Serial.printf("[UI] Temp: %.2f C\n", temp);
  if (timeValid) {
    Serial.printf("[UI] Time: %02d:%02d\n", timeinfo.tm_hour, timeinfo.tm_min);
  } else {
    Serial.println("[UI] Time: SYNCING...");
  }

  // Force redraw if temp changes heavily or first run
  bool forceBgRedraw = (abs(temp - lastTemp) > 0.5f);

  if (forceBgRedraw) {
    tft.fillScreen(bgColor);
    lastTemp = temp;
    lastHour = -1;
    lastMin = -1;
    lastColonVisible = !lastColonVisible;

    // Draw Temp (Bottom)
    tft.setCursor(10, 90);
    tft.setTextColor(ST77XX_YELLOW, bgColor);
    tft.setTextSize(3);
    if (isnan(temp)) {
      tft.print("--.- C");
    } else {
      tft.print(temp, 1);
      tft.print(" C");
    }
  }

  // Trosky (Moved to 190 to avoid Minutes overlap)
  drawTrosky(190, 80, listening, forceBgRedraw);

  if (timeValid) {
    int cursorY = 35;

    // HOURS
    if (timeinfo.tm_hour != lastHour || forceBgRedraw) {
      tft.fillRect(5, cursorY, 70, 40, bgColor);
      tft.setCursor(10, cursorY);
      tft.setTextColor(ST77XX_WHITE, bgColor);
      tft.setTextSize(5);
      if (timeinfo.tm_hour < 10)
        tft.print("0");
      tft.print(timeinfo.tm_hour);
      lastHour = timeinfo.tm_hour;
      // Redraw phase if hour changed
      drawDayPhase(200, 20, timeinfo.tm_hour);
    }

    // COLON (Size 5 char is ~30px wide)
    bool colonVisible = (millis() / 500) % 2 == 0;
    if (colonVisible != lastColonVisible || forceBgRedraw) {
      tft.fillRect(75, cursorY, 30, 40, bgColor); // Adjusted width
      if (colonVisible) {
        tft.setCursor(75, cursorY);
        tft.setTextColor(ST77XX_WHITE, bgColor);
        tft.setTextSize(5);
        tft.print(":");
      }
      lastColonVisible = colonVisible;
    }

    // MINUTES (Moved to 105/110)
    if (timeinfo.tm_min != lastMin || forceBgRedraw) {
      tft.fillRect(105, cursorY, 70, 40, bgColor);
      tft.setCursor(110, cursorY); // Shifted right
      tft.setTextColor(ST77XX_WHITE, bgColor);
      tft.setTextSize(5);
      if (timeinfo.tm_min < 10)
        tft.print("0");
      tft.print(timeinfo.tm_min);
      lastMin = timeinfo.tm_min;
    }
  } else {
    if (millis() % 1000 < 100) {
      tft.setCursor(10, 40);
      tft.setTextColor(ST77XX_WHITE, bgColor);
      tft.setTextSize(3);
      tft.print("SYNC...");
    }
  }
}

void BathroomUI::drawBackground(float temp, bool force) {
  // Simplified: Does nothing now, handled in update
}

uint16_t BathroomUI::interpolateColor(float temp, int hour) {
  // Night Mode (18:00 - 06:00) -> Dark Navy
  if (hour < 6 || hour >= 18) {
    return 0x000F; // Dark Navy Blue
  }

  // Day Mode -> Temp Gradient
  // Cold (Blue) -> Hot (Orange)
  // Map 15C to Blue, 30C to Orange
  if (temp < 15)
    return 0x001F; // Blue
  if (temp > 30)
    return 0xFD20; // Orange

  // Simple interpolation logic could go here, but stepping is safer for now
  return 0x03E0; // Dark Green-ish (Mid) or keep simpler
}

void BathroomUI::drawDayPhase(int x, int y, int hour) {
  tft.fillCircle(x, y, 16, ST77XX_BLACK); // Clear with Black
  if (hour >= 6 && hour < 18) {
    tft.fillCircle(x, y, 10, ST77XX_YELLOW);
  } else {
    tft.fillCircle(x, y, 10, ST77XX_WHITE);
  }
}

#include "app/modules/bathroom/TroskyBitmap.h"

void BathroomUI::drawTrosky(int x, int y, bool wag, bool forceRedraw) {
  static bool drawn = false;

  // Draw Bitmap Image
  if (forceRedraw || !drawn) {
    // Clear exact area
    tft.fillRect(x, y, TROSKY_WIDTH, TROSKY_HEIGHT,
                 interpolateColor(lastTemp, lastHour));

    // Draw Bitmap
    tft.drawRGBBitmap(x, y, troskyBitmap, TROSKY_WIDTH, TROSKY_HEIGHT);
    drawn = true;

    // Force background update on next Wag to prevent potential ghosting if
    // needed
  }
}

void BathroomUI::drawSOS() {
  static unsigned long lastBlink = 0;
  if (millis() - lastBlink > 500) {
    lastBlink = millis();
    bool on = (millis() / 500) % 2;
    tft.fillScreen(on ? ST77XX_RED : ST77XX_BLACK);
    if (on) {
      tft.setCursor(40, 50);
      tft.setTextColor(ST77XX_WHITE); // Transparent bg
      tft.setTextSize(3);
      tft.print("SOS");
    }
  }
}

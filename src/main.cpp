#ifdef NODE_BATHROOM
#include "app/modules/bathroom/BathroomNode.h"
#endif

#ifdef NODE_LIVING_ROOM
#include "app/modules/living_room/LivingRoomNode.h"
#endif

#include "engine/AppState.h"
#include "engine/EventBus.h"
#include "engine/ServiceLocator.h"
#include "services/WhatsAppService.h"
#include <Arduino.h>
#include <WiFi.h>
#include <esp_task_wdt.h>

// [SESSION PROTOCOL 2.114 - 4.3] Resource Management
// Non-blocking patterns and Watchdog Timers.

#ifdef NODE_BATHROOM
BathroomNode node;
#elif defined(NODE_LIVING_ROOM)
LivingRoomNode node;
#endif

void connectWiFi() {
  static unsigned long lastAttempt = 0;
  if (WiFi.status() != WL_CONNECTED && millis() - lastAttempt > 5000) {
    lastAttempt = millis();
    Serial.println("Connecting to WiFi...");
    WiFi.begin("YOUR_SSID", "YOUR_PASSWORD");
  }
}

void setup() {
  Serial.begin(115200);

  // [SESSION PROTOCOL 2.114 - 4.3] Failure Recovery
  esp_task_wdt_init(30, true); // 30 seconds timeout
  esp_task_wdt_add(NULL);

  WiFi.mode(WIFI_STA);

  // Initialize Services
  auto *bus = new EventBus();
  auto *state = new AppState();
  auto *whatsApp = new WhatsAppService("YOUR_PHONE", "YOUR_API_KEY");

  ServiceLocator::instance().registerService("EventBus", bus);
  ServiceLocator::instance().registerService("AppState", state);
  ServiceLocator::instance().registerService("WhatsAppService", whatsApp);

  // Manual Injection
  node.EventBus = bus;
  node.AppState = state;

  node.init();
  Serial.println("System Initialized");
}

void loop() {
  esp_task_wdt_reset();
  connectWiFi();
  node.loop();
}

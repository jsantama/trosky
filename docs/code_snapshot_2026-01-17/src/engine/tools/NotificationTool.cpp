#include "engine/tools/NotificationTool.h"

NotificationTool::NotificationTool(EventBus *bus, WhatsAppService *service)
    : eventBus(bus), whatsApp(service) {}

void NotificationTool::init() {
  if (eventBus) {
    eventBus->subscribe(EventType::SOS_TRIGGERED,
                        [this](const Event &e) { this->onSOS(e); });
    eventBus->subscribe(EventType::INTENT_TAXI,
                        [this](const Event &e) { this->onTaxi(e); });
    eventBus->subscribe(EventType::INTENT_RECIPE,
                        [this](const Event &e) { this->onRecipe(e); });
  }
}

void NotificationTool::onSOS(const Event &e) {
  if (!whatsApp) {
    Serial.println("[CRITICAL] WhatsApp service not available for SOS!");
    return;
  }
  if (!whatsApp->sendMessage("🚨 EMERGENCIA en el baño. Por favor revisen.")) {
    Serial.println("[ERROR] Failed to send SOS notification");
  }
}

void NotificationTool::onTaxi(const Event &e) {
  if (!whatsApp) {
    Serial.println("[ERROR] WhatsApp service not available for Taxi request");
    return;
  }
  if (!whatsApp->sendMessage("🚕 Solicitud de Taxi desde la Sala.")) {
    Serial.println("[WARN] Failed to send Taxi notification");
  }
}

void NotificationTool::onRecipe(const Event &e) {
  if (!whatsApp) {
    Serial.println("[ERROR] WhatsApp service not available for Recipe request");
    return;
  }
  if (!whatsApp->sendMessage("🍳 Receta sugerida enviada al grupo.")) {
    Serial.println("[WARN] Failed to send Recipe notification");
  }
}

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
  if (whatsApp) {
    whatsApp->sendMessage("🚨 EMERGENCIA en el baño. Por favor revisen.");
  }
}
void NotificationTool::onTaxi(const Event &e) {
  if (whatsApp) {
    whatsApp->sendMessage("🚕 Solicitud de Taxi desde la Sala.");
  }
}
void NotificationTool::onRecipe(const Event &e) {
  if (whatsApp) {
    whatsApp->sendMessage("🍳 Receta sugerida enviada al grupo.");
  }
}

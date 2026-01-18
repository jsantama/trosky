#include "engine/tools/NotificationTool.h"

NotificationTool::NotificationTool(EventBus *bus, WhatsAppService *service)
    : eventBus(bus), whatsApp(service) {}

void NotificationTool::init() {
  if (eventBus) {
    eventBus->subscribe(EventType::SOS_TRIGGERED,
                        [this](const Event &e) { this->onSOS(e); });
  }
}

void NotificationTool::onSOS(const Event &e) {
  if (whatsApp) {
    whatsApp->sendMessage("🚨 EMERGENCIA en el baño. Por favor revisen.");
  }
}

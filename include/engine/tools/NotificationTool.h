#ifndef NOTIFICATION_TOOL_H
#define NOTIFICATION_TOOL_H

#include "engine/EventBus.h"
#include "services/WhatsAppService.h"

/**
 * [SESSION PROTOCOL 2.114 - 3.3] Smart Tools
 * Bridges events to external services (notifications).
 */
class NotificationTool {
public:
  NotificationTool(EventBus *bus, WhatsAppService *service);
  void init();

private:
  EventBus *eventBus;
  WhatsAppService *whatsApp;

  void onSOS(const Event &e);
};

#endif

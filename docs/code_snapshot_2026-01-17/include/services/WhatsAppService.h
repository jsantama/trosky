#ifndef WHATSAPP_SERVICE_H
#define WHATSAPP_SERVICE_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <UrlEncode.h>

/**
 * [SESSION PROTOCOL 2.114 - 3.4] Dependency Management
 * Registered via ServiceLocator.
 */
class WhatsAppService {
public:
  WhatsAppService(const String &phone, const String &apiKey)
      : phone(phone), apiKey(apiKey) {}

  bool sendMessage(const String &message) {
    if (WiFi.status() != WL_CONNECTED)
      return false;

    HTTPClient http;
    String url = "http://api.callmebot.com/whatsapp.php?phone=" + phone +
                 "&text=" + urlEncode(message) + "&apikey=" + apiKey;

    http.begin(url);
    int httpCode = http.GET();
    http.end();

    return httpCode == 200;
  }

private:
  String phone;
  String apiKey;
};

#endif

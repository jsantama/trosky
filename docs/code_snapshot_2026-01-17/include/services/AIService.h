#ifndef I_AI_SERVICE_H
#define I_AI_SERVICE_H

#include <Arduino.h>

/**
 * [SESSION PROTOCOL 2.114 - 3.4] Dependency Management
 * Abstract interface for AI processing.
 */
class IAIService {
public:
  virtual ~IAIService() {}

  // Process audio text data or visual data
  virtual String processText(const String &prompt) = 0;
  virtual String identifyMedication(const uint8_t *imageData,
                                    size_t length) = 0;
};

#endif

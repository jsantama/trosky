#ifndef MOCK_AI_SERVICE_H
#define MOCK_AI_SERVICE_H

#include "services/AIService.h"

class MockAIService : public IAIService {
public:
  String processText(const String &prompt) override;
  String identifyMedication(const uint8_t *imageData, size_t length) override;
};

#endif

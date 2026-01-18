#include "services/MockAIService.h"

String MockAIService::processText(const String &prompt) {
  Serial.printf("AI Processing: %s\n", prompt.c_str());
  return "Respuesta simulada de Trosky para: " + prompt;
}

String MockAIService::identifyMedication(const uint8_t *imageData,
                                         size_t length) {
  Serial.println("AI Identifying Medication from image...");
  return "Medicina detectada: Paracetamol (Simulado)";
}

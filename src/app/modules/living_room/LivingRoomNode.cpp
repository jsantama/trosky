#include "app/modules/living_room/LivingRoomNode.h"
#include "HAL.h"
#include "engine/ServiceLocator.h"
#include <driver/i2s.h>

#define SAMPLE_RATE 16000

LivingRoomNode::LivingRoomNode()
    : EventBus(nullptr), AppState(nullptr), whatsApp(nullptr) {}

void LivingRoomNode::init() {
  // WhatsApp Service Init from ServiceLocator
  whatsApp = ServiceLocator::instance().get<WhatsAppService>("WhatsAppService");

  // I2S Mic Init
  setupI2S();

  Serial.println("Living Room Node Initialized");
}

void LivingRoomNode::loop() {
  // Voice Recognition Loop (Simplified)
  if (detectWakeWord()) {
    Serial.println("Wake word 'Trosky' detected!");
    if (EventBus)
      EventBus->publish(EventType::WAKE_WORD_DETECTED);

    String intent = captureIntent();
    if (intent == "taxi") {
      processTaxiRequest();
    } else if (intent == "recipe") {
      processRecipeRequest();
    }
  }
}

void LivingRoomNode::setupI2S() {
  i2s_config_t i2s_config = {.mode =
                                 (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
                             .sample_rate = SAMPLE_RATE,
                             .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
                             .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
                             .communication_format = I2S_COMM_FORMAT_STAND_I2S,
                             .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
                             .dma_buf_count = 8,
                             .dma_buf_len = 64,
                             .use_apll = false};

  i2s_pin_config_t pin_config = {.bck_io_num = HAL::LivingRoom::I2S_SCK,
                                 .ws_io_num = HAL::LivingRoom::I2S_WS,
                                 .data_out_num = I2S_PIN_NO_CHANGE,
                                 .data_in_num = HAL::LivingRoom::I2S_SD};

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
}

bool LivingRoomNode::detectWakeWord() {
  // Placeholder for actual detection
  return false;
}

String LivingRoomNode::captureIntent() {
  // Placeholder for intent recognition
  return "";
}

void LivingRoomNode::processTaxiRequest() {
  if (whatsApp)
    whatsApp->sendMessage("🚕 Solicitud de Taxi desde la Sala.");
  if (EventBus)
    EventBus->publish(EventType::INTENT_TAXI);
}

void LivingRoomNode::processRecipeRequest() {
  if (whatsApp)
    whatsApp->sendMessage("🍳 Receta sugerida: [Detalle de la receta]");
  if (EventBus)
    EventBus->publish(EventType::INTENT_RECIPE);
}

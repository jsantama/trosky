#include "app/modules/living_room/LivingRoomNode.h"
#include "HAL.h"
#include "engine/ServiceLocator.h"
#include <driver/i2s.h>
#include <esp_camera.h>

#define SAMPLE_RATE 16000

LivingRoomNode::LivingRoomNode()
    : EventBus(nullptr), AppState(nullptr), whatsApp(nullptr),
      notificationTool(nullptr) {}

LivingRoomNode::~LivingRoomNode() {
  // [SESSION PROTOCOL 2.114 - Memory Management]
  delete notificationTool;
  // Note: whatsApp is managed by ServiceLocator, don't delete
}

void LivingRoomNode::init() {
  // WhatsApp Service Init from ServiceLocator
  whatsApp = ServiceLocator::instance().get<WhatsAppService>("WhatsAppService");
  notificationTool = new NotificationTool(EventBus, whatsApp);
  notificationTool->init();

  // I2S Mic Init
  setupI2S();

  // Camera Init [SESSION PROTOCOL 4.3 - HAL Mapping]
  if (setupCamera()) {
    Serial.println("Camera Initialized");
  } else {
    Serial.println("Camera Initialization Failed");
  }

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
  if (EventBus)
    EventBus->publish(EventType::INTENT_TAXI);
}

void LivingRoomNode::processRecipeRequest() {
  if (EventBus)
    EventBus->publish(EventType::INTENT_RECIPE);
}

bool LivingRoomNode::setupCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = HAL::LivingRoom::CAM_D0;
  config.pin_d1 = HAL::LivingRoom::CAM_D1;
  config.pin_d2 = HAL::LivingRoom::CAM_D2;
  config.pin_d3 = HAL::LivingRoom::CAM_D3;
  config.pin_d4 = HAL::LivingRoom::CAM_D4;
  config.pin_d5 = HAL::LivingRoom::CAM_D5;
  config.pin_d6 = HAL::LivingRoom::CAM_D6;
  config.pin_d7 = HAL::LivingRoom::CAM_D7;
  config.pin_xclk = HAL::LivingRoom::CAM_XCLK;
  config.pin_pclk = HAL::LivingRoom::CAM_PCLK;
  config.pin_vsync = HAL::LivingRoom::CAM_VSYNC;
  config.pin_href = HAL::LivingRoom::CAM_HREF;
  config.pin_sscb_sda = HAL::LivingRoom::CAM_SDA;
  config.pin_sscb_scl = HAL::LivingRoom::CAM_SCL;
  config.pin_pwdn = HAL::LivingRoom::CAM_PWDN;
  config.pin_reset = HAL::LivingRoom::CAM_RESET;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  return (err == ESP_OK);
}

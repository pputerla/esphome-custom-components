#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/log.h"


#ifdef USE_LOGGER
#include "esphome/components/logger/logger.h"
#endif

namespace esphome {
namespace s3km1110 {


class S3KM1110Component : public Component, public uart::UARTDevice {
 public:
  S3KM1110Component() {
      ESP_LOGD("ble", "s3km1110 constructor!");
  }
  void setup() override;
  void loop() override;
};

}
}
#include "esphome.h"


namespace esphome {
namespace s3km1110 {

class S3KM1110Component : public Component, public uart::UARTDevice {
 public:
  void setup() override {
    // This will be called once to set up the component
    // think of it as the setup() call in Arduino
    ESP_LOGD("custom", "Component started");


  }
  void loop() override {
    // This will be called very often after setup time.
    // think of it as the loop() call in Arduino
    /*if (digitalRead(5)) {
      digitalWrite(6, HIGH);

      // You can also log messages
      ESP_LOGD("custom", "The GPIO pin 5 is HIGH!");
    }*/
  }
};

}
}
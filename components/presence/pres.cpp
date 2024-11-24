#include "esphome.h"

class MyCustomComponent : public Component {
 public:
  void setup() override {
    // This will be called once to set up the component
    // think of it as the setup() call in Arduino

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
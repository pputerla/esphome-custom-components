#include "esphome.h"
#include "s3km1110.h"


namespace esphome {
namespace s3km1110 {

  static const char *const TAG = "s3km1110";


  void S3KM1110Component::setup()  {
    // This will be called once to set up the component
    // think of it as the setup() call in Arduino

      ESP_LOGD(TAG, "Initializing s3km1110...");

  }
  void S3KM1110Component::loop()  {
    // This will be called very often after setup time.
    // think of it as the loop() call in Arduino
    /*if (digitalRead(5)) {
      digitalWrite(6, HIGH);

      // You can also log messages
      ESP_LOGD("custom", "The GPIO pin 5 is HIGH!");
    }*/
  }

}
}
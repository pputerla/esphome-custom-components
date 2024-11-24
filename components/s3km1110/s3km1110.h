#include "esphome.h"


class S3KM1110Component : public Component, public uart::UARTDevice {
 public:
  void setup() override;
  void loop() override;
};

}
}
#include "esphome.h"

namespace esphome {
namespace s3km1110 {


class S3KM1110Component : public Component, public uart::UARTDevice {
 public:
  void setup() override;
  void loop() override;
};

}
}
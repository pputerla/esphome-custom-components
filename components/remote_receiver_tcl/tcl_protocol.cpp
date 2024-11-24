#include "tcl_protocol.h"
#include "esphome/core/log.h"

namespace esphome {
namespace remote_base {

static const char *const TAG = "remote.tcl";

static const uint32_t HEADER_HIGH_US = 4000;
static const uint32_t HEADER_LOW_US = 4000;
static const uint32_t BIT_HIGH_US = 500;
static const uint32_t BIT_ONE_LOW_US = 1000;
static const uint32_t BIT_ZERO_LOW_US = 2000;
static const uint32_t BIT_FOOTER_LOW_US = 8500;

void TCLProtocol::encode(RemoteTransmitData *dst, const TCLData &data) {
  ESP_LOGD(TAG, "Sending TCL: data=0x%jx", data.data);

  dst->reserve(2 + 48 + 2);
  dst->set_carrier_frequency(38000);
  dst->item(HEADER_HIGH_US, HEADER_LOW_US);
    uint32_t mask = 1;
    for (int i=0; i<24; mask <<= 1) {
      if (data.data & mask) {
        dst->item(BIT_HIGH_US, BIT_ONE_LOW_US);
      } else {
        dst->item(BIT_HIGH_US, BIT_ZERO_LOW_US);
      }
    }
    dst->item(HEADER_HIGH_US, BIT_FOOTER_LOW_US);
}
optional<TCLData> TCLProtocol::decode(RemoteReceiveData src) {
  TCLData ndata;
  ndata.data=0;
  if (!src.expect_item(HEADER_HIGH_US, HEADER_LOW_US))
    return {};

  uint32_t mask = 1;
  for (int i=0; i<24; mask <<= 1) {
    if (src.expect_item(BIT_HIGH_US, BIT_ONE_LOW_US)) {
      ndata.data |= mask;
    } else if (src.expect_item(BIT_HIGH_US, BIT_ZERO_LOW_US)) {
      ndata.data &= ~mask;
    } else if (src.expect_item(BIT_HIGH_US, BIT_FOOTER_LOW_US)) {
      break;
    } else {
      return {};
    }
  }
  return ndata;
}
void TCLProtocol::dump(const TCLData &ndata) {
  ESP_LOGI(TAG, "Received TCL: data=0x%lx",  ndata.data);
}

}  // namespace remote_base
}  // namespace esphome

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
  ESP_LOGD(TAG, "Sending TCL: data=0x%jx command_repeats=%d", data.data,
           data.command_repeats);

  dst->reserve(2 + 32 + 32 * data.command_repeats + 2);
  dst->set_carrier_frequency(38000);

  dst->item(HEADER_HIGH_US, HEADER_LOW_US);

  /*for (uint16_t mask = 1; mask; mask <<= 1) {
    if (data.address & mask) {
      dst->item(BIT_HIGH_US, BIT_ONE_LOW_US);
    } else {
      dst->item(BIT_HIGH_US, BIT_ZERO_LOW_US);
    }
  }*/

  for (uint16_t repeats = 0; repeats < data.command_repeats; repeats++) {
    for (uint64_t mask = 1; mask; mask <<= 1) {
      if (data.data & mask) {
        dst->item(BIT_HIGH_US, BIT_ONE_LOW_US);
      } else {
        dst->item(BIT_HIGH_US, BIT_ZERO_LOW_US);
      }
    }
    //todo mark end 17
  }

  dst->mark(BIT_HIGH_US);
  //todo mark end full
}
optional<TCLData> TCLProtocol::decode(RemoteReceiveData src) {
  TCLData ndata{
      .data = 0,
      .command_repeats = 1,
  };
  if (!src.expect_item(HEADER_HIGH_US, HEADER_LOW_US))
    return {};


  for (uint64_t mask = 1; mask; mask <<= 1) {
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

  while (src.expect_item(HEADER_HIGH_US, HEADER_LOW_US)) {
    uint64_t data = 0;
    for (uint64_t mask = 1; mask; mask <<= 1) {
      if (src.expect_item(BIT_HIGH_US, BIT_ONE_LOW_US)) {
        data |= mask;
      } else if (src.expect_item(BIT_HIGH_US, BIT_ZERO_LOW_US)) {
        data &= ~mask;
      } else if (src.expect_item(BIT_HIGH_US, BIT_FOOTER_LOW_US)) {
        break;
      } else {
        ESP_LOGD(TAG, "code in a sequence did not follow the TCL pattern; discarding whole sequence");
        return {};
      }
    }


    // Make sure the extra/repeated data matches original command
    if (data != ndata.data) {
      //ESP_LOGD(TAG, "eh repeated count %d but somthing did not match", ndata.command_repeats);
      return {};
    }

    ndata.command_repeats += 1;
  }

  src.expect_mark(BIT_HIGH_US);
  return ndata;
}
void TCLProtocol::dump(const TCLData &ndata) {
  ESP_LOGI(TAG, "Received TCL: data=0x%llx command_repeats=%d",  ndata.data,
           ndata.command_repeats);
}

}  // namespace remote_base
}  // namespace esphome

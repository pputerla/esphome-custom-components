#pragma once

#include "esphome/components/remote_base/remote_base.h"

namespace esphome {
namespace remote_base {

struct TCLData {
  uint64_t data;
  uint16_t command_repeats;

  bool operator==(const TCLData &rhs) const { return data == rhs.data; }
};

class TCLProtocol : public RemoteProtocol<TCLData> {
 public:
  void encode(RemoteTransmitData *dst, const TCLData &data) override;
  optional<TCLData> decode(RemoteReceiveData src) override;
  void dump(const TCLData &data) override;
};

DECLARE_REMOTE_PROTOCOL(TCL)

template<typename... Ts> class TCLAction : public RemoteTransmitterActionBase<Ts...> {
 public:
  TEMPLATABLE_VALUE(uint64_t, data)
  TEMPLATABLE_VALUE(uint16_t, command_repeats)

  void encode(RemoteTransmitData *dst, Ts... x) override {
    TCLData data{};
    data.data = this->data_.value(x...);
    data.command_repeats = this->command_repeats_.value(x...);
    TCLProtocol().encode(dst, data);
  }
};

}  // namespace remote_base
}  // namespace esphome

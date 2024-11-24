#pragma once

#include "esphome/components/remote_base/remote_base.h"

namespace esphome {
namespace remote_base {

struct TCLData {
  unsigned int data : 24;

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
  TEMPLATABLE_VALUE(uint32_t, data)

  void encode(RemoteTransmitData *dst, Ts... x) override {
    TCLData data{};
    data.data = this->data_.value(x...);
    TCLProtocol().encode(dst, data);
  }
};

}  // namespace remote_base
}  // namespace esphome

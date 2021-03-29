#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

class ThrottleDisable_0x91 
  : public OsccProtocolBase,
    public ::apollo::drivers::canbus::ProtocolData<::apollo::canbus::ChassisDetail> {
 public:
  static const int32_t ID = 0x91;

  ThrottleDisable_0x91();

  uint32_t GetPeriod() const override;

  void UpdateData(uint8_t *data) override;

  void Reset() override;

  ThrottleDisable_0x91 *set_brake_disable();

 private:
   bool throttle_disable_ = false;
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

class ThrottleEnable_0x90 
  : public OsccProtocolBase,
    public ::apollo::drivers::canbus::ProtocolData<::apollo::canbus::ChassisDetail> {
 public:
  static const int32_t ID = 0x90;

  ThrottleEnable_0x90();

  uint32_t GetPeriod() const override;

  void UpdateData(uint8_t* data) override;

  void Reset() override;

  ThrottleEnable_0x90 *set_brake_enable();

 private:
   bool throttle_enable_ = false;
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

class BrakeDisable_0x71 
  : public OsccProtocolBase,
    public ::apollo::drivers::canbus::ProtocolData<::apollo::canbus::ChassisDetail> {
 public:
  static const int32_t ID;

  BrakeDisable_0x71();

  uint32_t GetPeriod() const override;

  void UpdateData(uint8_t* data) override;

  void Reset() override;

  BrakeDisable_0x71* set_brake_disable();

 private:
   bool brake_disable_ = false;
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
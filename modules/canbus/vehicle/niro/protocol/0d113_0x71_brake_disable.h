#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

// From the KIA Niro DBC file:
// BO_ 113 BRAKE_DISABLE: 8 BRAKE
//    SG_ brake_disable_magic : 0|16@1+ (1,0) [0|0] "" BRAKE

class BrakeDisable_0x71 : public OsccProtocolBase {
 public:
  static const int32_t ID = 0x71;

  BrakeDisable_0x71();

  uint32_t GetPeriod() const override;

  void UpdateData(uint8_t *data) override;

  void Reset() override;

  BrakeDisable_0x71 *set_brake_disable();

 private:
   bool brake_disable_ = false;
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
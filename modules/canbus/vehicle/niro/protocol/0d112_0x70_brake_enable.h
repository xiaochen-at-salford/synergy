#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

/* From the KIA Niro DBC file:
 * BO_ 112 BRAKE_ENABLE: 8 BRAKE
 *    BO_SG_ brake_enable_magic : 0|16@1+ (1,0) [0|0] "" BRAKE
 */

class BrakeEnable_0x70 : public OsccProtocolBase {
 public:
  static const int32_t ID = 0x70;

  BrakeEnable_0x70();

  uint32_t GetPeriod() const override;

  void UpdateData(uint8_t* data) override;

  void Reset() override;

  BrakeEnable_0x70 *set_brake_enable();
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
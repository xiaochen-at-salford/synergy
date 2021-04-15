#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

/* From the KIA Niro DBC file:
 * BO_ 129 STEERING_DISABLE: 8 STEERING
 *    SG_ steering_disable_magic : 0|16@1+ (1,0) [0|0] "" STEERING
 */

class SteeringDisable_0x81 : public OsccProtocolBase {
 public:
  static const int32_t ID = 0x81;

  SteeringDisable_0x81();

  uint32_t GetPeriod() const override;

  void UpdateData(uint8_t *data) override;

  void Reset() override;

  SteeringDisable_0x81 *set_steering_disable();
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
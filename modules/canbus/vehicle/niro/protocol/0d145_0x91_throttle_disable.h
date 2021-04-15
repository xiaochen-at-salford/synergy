#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

/* From the KIA Niro DBC file:
 * BO_ 128 STEERING_ENABLE: 8 STEERING
 *     SG_ steering_enable_magic : 0|16@1+ (1,0) [0|0] "" STEERING
 */

class ThrottleDisable_0x91 : public OsccProtocolBase {
 public:
  static const int32_t ID = 0x91;

  ThrottleDisable_0x91();

  uint32_t GetPeriod() const override;

  void UpdateData(uint8_t *data) override;

  void Reset() override;

  ThrottleDisable_0x91 *set_throttle_disable();
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
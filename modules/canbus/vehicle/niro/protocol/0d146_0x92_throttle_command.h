#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

/* From the KIA Niro DBC file:
 * BO_ 146 THROTTLE_COMMAND: 8 THROTTLE
 *    SG_ throttle_command_magic : 0|16@1+ (1,0) [0|0] "" THROTTLE
 *    SG_ throttle_pedal_command : 16|32@1+ (1,0) [0|1] "percent" THROTTLE
 */

class ThrottleCommand_0x92 : public OsccProtocolBase {
 public:
  static const int32_t ID = 0x92;

  ThrottleCommand_0x92();

  uint32_t GetPeriod() const override;

  void UpdateData(uint8_t* data) override;

  void Reset() override;

  /**
   * @brief Set the brake pedal command
   * 
   * @param throttle_pedal_position The percentage of pedal position [0~100]
   * @return BrakeCommand_0x92* 
   */
  ThrottleCommand_0x92* set_throttle_pedal_command(double throttle_pedal_percent);

 private:
  void set_p_throttle_pedal_command(uint8_t *data, double throttle_pedal_percent);

 private:
  double throttle_pedal_percent_;
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo

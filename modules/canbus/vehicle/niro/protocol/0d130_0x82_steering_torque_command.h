#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

/* From the KIA Niro DBC file:
 * BO_ 130 STEERING_TORQUE_COMMAND: 8 STEERING
 *    SG_ steering_command_magic : 0|16@1+ (1,0) [0|0] "" STEERING
 *    SG_ steering_torque_command : 16|32@1- (1,0) [-1|1] "percent" STEERING
 */

class SteeringTorqueCommand_0x82 : public OsccProtocolBase {
 public:
  static const int32_t ID = 0x82;

  SteeringTorqueCommand_0x82();

  uint32_t GetPeriod() const override;

  void UpdateData(uint8_t* data) override;

  void Reset() override;

  /**
   * @brief Set the brake pedal command
   * 
   * @param brake_pedal_position The percentage of pedal position [0~100]
   * @return BrakeCommand_0x72* 
   */
  SteeringTorqueCommand_0x82* set_steering_torque_percent(double steering_torque_percent);

 private:

  void set_p_steering_torque_command(uint8_t *data, double steering_torque_percent);

 private:
  double steering_torque_percent_; // [%]:[0.0, 100.0]
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo

#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

class SteeringAngleCommand_0xB8 : public OsccProtocolBase {
 public:
  static const int32_t ID = 0xB8;

  SteeringAngleCommand_0xB8();

  uint32_t GetPeriod() const override;

  void UpdateData(uint8_t *data) override;

  void Reset() override;

  SteeringAngleCommand_0xB8 *set_steering_angle_command_flags(bool steering_angle_cmd_flags);

  SteeringAngleCommand_0xB8 *set_steering_angle_percent(double steering_angle_percent);

  SteeringAngleCommand_0xB8 *set_max_steering_velocity_percent(double max_steering_velocity_percent);

 private:
  void set_p_steering_angle_cmd_flags(uint8_t *data, bool steering_angle_cmd_flags);

  void set_p_steering_angle_cmd_angle(uint8_t *data, double steering_angle_percent);

  void set_p_steering_angle_cmd_max_velocity(uint8_t *data, double steering_angle_max_velocity);

 private:
  bool steering_angle_cmd_flags_ =  false;
  double steering_angle_percent_ = 0.0; // [%]:[-100.0, 100.0]
  double max_steering_velocity_percent_ = 0.0; // [%]:[0, 100]
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo

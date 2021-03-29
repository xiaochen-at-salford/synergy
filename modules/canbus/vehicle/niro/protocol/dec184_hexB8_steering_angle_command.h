#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

/* From OSCC SDK:
 * typedef struct {
 *   uint8_t magic[2]; // Magic number identifying CAN frame as from OSCC.
 *                     // Byte 0 should be \ref OSCC_MAGIC_BYTE_0.
 *                     // Byte 1 should be \ref OSCC_MAGIC_BYTE_1.
 *   float pedal_command; // Brake Request 0.0 to 1.0 where 1.0 is 100% 
 *   uint8_t reserved[2]; // Reserved. 
 * } oscc_brake_command_s;
 */

typedef Steering_Angle_Command_0xB8::SteeringAngleCmdFlags::Enum SteeringAngleCmdFlagsEnum; 

class SteeringAngleCommand_0xB8 
  : public OsccProtocolBase,
    public ::apollo::drivers::canbus::ProtocolData<::apollo::canbus::ChassisDetail> {
 public:
  static const int32_t ID = 0xB8;

  SteeringAngleCommand_0xB8();

  uint32_t GetPeriod() const override;

  void UpdateData(uint8_t *data) override;

  void Reset() override;

  SteeringAngleCommand_0xB8 *set_steering_angle_command_flags(SteeringAngleCmdFlagsEnum steering_angle_cmd_flags);

  SteeringAngleCommand_0xB8 *set_steering_angle_commmand(double steering_angle_percent);

  SteeringAngleCommand_0xB8 *set_max_steering_velocity(double max_steering_velocity_percent);

 private:
  void set_p_steering_angle_cmd_flags(uint8_t *data, SteeringAngleCmdFlagsEnum steering_angle_cmd_flags);

  void set_p_steering_angle_cmd_angle(uint8_t *data, double steering_angle_percent);

  void set_p_steering_angle_cmd_max_velocity(uint8_t *data, double steering_angle_max_velocity);

 private:
  //TODO(xiaochen)
  SteeringAngleCmdFlagsEnum steering_angle_cmd_flags_ = 
     SteeringAngleCmdFlagsEnum::Steering_Angle_Command_0xB8_SteeringAngleCmdFlags_Enum_DISABLED ;
  double steering_angle_percent_ = 0.0; // [-100, 100]
  double max_steering_velocity_percent_ = 0.0; // [0, 100]
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo

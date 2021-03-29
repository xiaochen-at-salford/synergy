#include "modules/canbus/vehicle/niro/protocol/dec184_hexB8_steering_angle_command.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

// public
SteeringAngleCommand_0xB8::SteeringAngleCommand_0xB8() { Reset(); }

uint32_t SteeringAngleCommand_0xB8::GetPeriod() const 
{
  // TODO(All) :  modify every protocol's period manually
  static const uint32_t PERIOD = 20*1000;
  return PERIOD;
}


void SteeringAngleCommand_0xB8::UpdateData(uint8_t *data) 
{
  set_p_steering_angle_cmd_flags(data, steering_angle_cmd_flags_);
  set_p_steering_angle_cmd_angle(data, steering_angle_percent_);
  set_p_steering_angle_cmd_max_velocity(data, max_steering_velocity_percent_);
}

void SteeringAngleCommand_0xB8::Reset() 
{
  steering_angle_cmd_flags_ = 
     SteeringAngleCmdFlagsEnum::Steering_Angle_Command_0xB8_SteeringAngleCmdFlags_Enum_DISABLED ;
  steering_angle_percent_ = 0.0;
  max_steering_velocity_percent_ = 0.0;
}

SteeringAngleCommand_0xB8 *
SteeringAngleCommand_0xB8::set_steering_angle_command_flags(SteeringAngleCmdFlagsEnum steering_angle_cmd_flags) 
{
  steering_angle_cmd_flags_  = steering_angle_cmd_flags;
  return this;
}

SteeringAngleCommand_0xB8 *SteeringAngleCommand_0xB8::set_steering_angle_commmand(double steering_angle_percent) 
{
  steering_angle_percent_ = steering_angle_percent;
  return this;
}

SteeringAngleCommand_0xB8 *SteeringAngleCommand_0xB8::set_max_steering_velocity(double max_steering_velocity_percent) 
{
  max_steering_velocity_percent_ = max_steering_velocity_percent;
  return this;
}


void SteeringAngleCommand_0xB8::set_p_steering_angle_cmd_flags(
      uint8_t *data, SteeringAngleCmdFlagsEnum steering_angle_cmd_flags )
{
  float x = static_cast<SteeringAngleCmdFlagsEnum>(steering_angle_cmd_flags);
  memcpy(data, &x, sizeof(x));
}

void SteeringAngleCommand_0xB8::set_p_steering_angle_cmd_angle(uint8_t *data, double steering_angle_percent) 
{
  steering_angle_percent = ProtocolData::BoundedValue(-100.0, 100.0, steering_angle_percent);
  double scale = 0.1;
  double max = 600.0;
  double x = steering_angle_percent / 100.0 * max / scale;

  /// Big endian
  int16_t y = static_cast<int16_t>(x);

  memcpy(data+1, &y, sizeof(y));
}

void SteeringAngleCommand_0xB8::set_p_steering_angle_cmd_max_velocity(uint8_t *data, double max_steering_velocity_percent)
{
  max_steering_velocity_percent = ProtocolData::BoundedValue(0.0, 100.0, max_steering_velocity_percent);
  double scale = 0.1;
  double max = 800.0;
  double x = max_steering_velocity_percent / 100.0 * max / scale;

  /// Big endian
  int16_t y = static_cast<int16_t>(x);

  memcpy(data+3, &y, sizeof(y));
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
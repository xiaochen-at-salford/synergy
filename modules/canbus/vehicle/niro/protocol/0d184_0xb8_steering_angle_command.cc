#include "modules/canbus/vehicle/niro/protocol/0d184_0xb8_steering_angle_command.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

SteeringAngleCommand_0xB8::SteeringAngleCommand_0xB8() 
{ Reset(); }

uint32_t SteeringAngleCommand_0xB8::GetPeriod() 
const {
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
  disable_magic_use();
  enable_auto_activation();
  steering_angle_cmd_flags_ = false;
  steering_angle_percent_ = 0.0;
  max_steering_velocity_percent_ = 0.0;
}

SteeringAngleCommand_0xB8 *SteeringAngleCommand_0xB8::set_steering_angle_command_flags(bool steering_angle_cmd_flags) 
{
  steering_angle_cmd_flags_  = steering_angle_cmd_flags;
  return this;
}

SteeringAngleCommand_0xB8 *SteeringAngleCommand_0xB8::set_steering_angle_percent(double steering_angle_percent) 
{
  steering_angle_percent_ = steering_angle_percent;
  return this;
}

SteeringAngleCommand_0xB8 *SteeringAngleCommand_0xB8::set_max_steering_velocity_percent(double max_steering_velocity_percent) 
{
  max_steering_velocity_percent_ = max_steering_velocity_percent;
  return this;
}

void SteeringAngleCommand_0xB8::set_p_steering_angle_cmd_flags(uint8_t *data, bool enable)
{ data[0] = static_cast<uint8_t>(enable); }

void SteeringAngleCommand_0xB8::set_p_steering_angle_cmd_angle(uint8_t *data, double steering_angle_percent) 
{
  float scale = 10.0;
  float angle = ProtocolData::BoundedValue(-100.0, 100.0, steering_angle_percent) / 100.0 * 600.0;
  int16_t x = static_cast<int16_t>(scale*angle);
  std::memcpy(&data[1], &angle, sizeof(x));
}

void SteeringAngleCommand_0xB8::set_p_steering_angle_cmd_max_velocity(uint8_t *data, double max_steering_velocity_percent)
{
  float max = 800.0;
  float max2 = ProtocolData::BoundedValue(0.0, 100.0, max_steering_velocity_percent) / 100.0 * max;
  float scale = 10.0;
  int16_t max_vel = static_cast<int16_t>(scale*max2);
  std::memcpy(&data[3], &max_vel, sizeof(max_vel));
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
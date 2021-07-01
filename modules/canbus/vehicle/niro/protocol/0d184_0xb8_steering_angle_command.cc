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
  static const uint32_t PERIOD = 33*1000; 
  return PERIOD;
}

void SteeringAngleCommand_0xB8::UpdateData(uint8_t *data) 
{
  // data[0] = 1;
  // float scale = 10;
  // int16_t angle = static_cast<int16_t>(scale*steering_angle_percent_);
  // // int16_t angle = static_cast<int16_t>(scale*60);
  // std::memcpy(&(data[1]), &angle, sizeof(angle));
  // int16_t max_vel = static_cast<int16_t>(scale*30);
  // std::memcpy(&(data[3]), &max_vel, sizeof(max_vel));

  set_p_steering_angle_cmd_flags(data, steering_angle_cmd_flags_);
  set_p_steering_angle_cmd_angle(data, steering_angle_percent_);
  set_p_steering_angle_cmd_max_velocity(data, max_steering_velocity_percent_);

  // printf("update data steering angle percent %f\n",  steering_angle_percent_);
}

void SteeringAngleCommand_0xB8::Reset() 
{
  activate();
  enable_auto_activation();
  disable_magic_use();
  steering_angle_cmd_flags_ = true;
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
  // | Niro (degrees) | OSCC DBC (degees) | APOLLO (percent) | 
  // | (-485, 485)    | (-600, 600)       | (-100, 100)      |
  // float angle = ProtocolData::BoundedValue(-100.0, 100.0, steering_angle_percent);
  double apollo_scale = 3;
  steering_angle_percent *= apollo_scale;
  float angle = ProtocolData::BoundedValue(-25.0, 25.0, steering_angle_percent);
  angle = angle / 100.0 * 470.0;
  float scale = 10.0; // Defined by OSCC
  int16_t x = static_cast<int16_t>(scale*angle);
  // int16_t x = static_cast<int16_t>(scale*60);
  // "+" : Left   "-": Right
  x *= -1;
  std::memcpy(&data[1], &x, sizeof(x));
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
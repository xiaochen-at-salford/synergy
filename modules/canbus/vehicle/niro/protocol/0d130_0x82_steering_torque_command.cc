#include "modules/canbus/vehicle/niro/protocol/0d130_0x82_steering_torque_command.h"

namespace apollo {
namespace canbus {
namespace niro {

// using ::apollo::drivers::canbus::Byte;

SteeringTorqueCommand_0x82::SteeringTorqueCommand_0x82() 
{
  enable_magic_use();
  enable_auto_activation();
  Reset(); 
}

uint32_t SteeringTorqueCommand_0x82::GetPeriod() const 
{
  static const uint32_t PERIOD = 20*1000;
  return PERIOD;
}

void SteeringTorqueCommand_0x82::UpdateData(uint8_t *data) 
{
  set_p_magic(data);
  set_p_steering_torque_command(data, steering_torque_percent_);
}

void SteeringTorqueCommand_0x82::Reset() 
{ steering_torque_percent_ = 0.0; }

SteeringTorqueCommand_0x82 *SteeringTorqueCommand_0x82::set_steering_torque_percent(double steering_torque_percent) 
{
  steering_torque_percent_ = steering_torque_percent;
  return this;
}

void SteeringTorqueCommand_0x82::set_p_steering_torque_command(uint8_t *data, double steering_torque_percent) 
{
  steering_torque_percent = ProtocolData::BoundedValue(0.0, 100.0, steering_torque_percent);
  // For OSCC, the range of pedal command is [0,1]
  float x = static_cast<float>(steering_torque_percent / 100.0);
  memcpy(&data[2], &x, sizeof(x));
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
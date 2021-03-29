#include "modules/canbus/vehicle/niro/protocol/dec130_hex82_steering_torque_command.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

// const int32_t BrakeCommand_0x72::ID = 0x72;

// public
SteeringTorqueCommand_0x82::SteeringTorqueCommand_0x82() { Reset(); }

uint32_t SteeringTorqueCommand_0x82::GetPeriod() const 
{
  // TODO(All) :  modify every protocol's period manually
  static const uint32_t PERIOD = 20*1000;
  return PERIOD;
}

void SteeringTorqueCommand_0x82::UpdateData(uint8_t *data) 
{
  set_p_magic(data);
  set_p_steering_torque_command(data, steering_torque_percent_);
}

void SteeringTorqueCommand_0x82::Reset() 
{
  // TODO(All) :  you should check this manually
  steering_torque_percent_ = 0.0;
}

SteeringTorqueCommand_0x82 *SteeringTorqueCommand_0x82::set_steering_torque_command(double steering_torque_percent) 
{
  steering_torque_percent_ = steering_torque_percent;
  return this;
}

//TODO(xiaochen):
void SteeringTorqueCommand_0x82::set_p_steering_torque_command(uint8_t *data, double steering_torque_percent) 
{
  steering_torque_percent = ProtocolData::BoundedValue(0.0, 100.0, steering_torque_percent);

  // For OSCC, the range ofpedal command is [0,1]
  float x = static_cast<float>(steering_torque_percent / 100.0);

  //TODO(xiaochen): Check wether Big Endian or Little Endia
  // The "pedal_command" starts from the 3rd byte of "oscc_brake_command_s"
  memcpy(data+2, &x, sizeof(x));
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
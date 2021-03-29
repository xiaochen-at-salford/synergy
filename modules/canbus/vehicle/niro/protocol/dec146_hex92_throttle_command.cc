#include "modules/canbus/vehicle/niro/protocol/dec146_hex92_throttle_command.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

// const int32_t BrakeCommand_0x72::ID = 0x72;

// public
ThrottleCommand_0x92::ThrottleCommand_0x92() { Reset(); }

uint32_t ThrottleCommand_0x92::GetPeriod() const 
{
  // TODO(All) :  modify every protocol's period manually
  static const uint32_t PERIOD = 20*1000;
  return PERIOD;
}

void ThrottleCommand_0x92::UpdateData(uint8_t *data) 
{
  set_p_magic(data);
  set_p_throttle_pedal_command(data, throttle_pedal_percent_);
}

void ThrottleCommand_0x92::Reset() 
{
  // TODO(All) :  you should check this manually
  throttle_pedal_percent_ = 0.0;
}

ThrottleCommand_0x92 *ThrottleCommand_0x92::set_throttle_pedal_command(double throttle_pedal_percent) 
{
  throttle_pedal_percent_ = throttle_pedal_percent;
  return this;
}

void ThrottleCommand_0x92::set_p_throttle_pedal_command(uint8_t *data, double throttle_pedal_percent) 
{
  throttle_pedal_percent = ProtocolData::BoundedValue(0.0, 100.0, throttle_pedal_percent);

  // For OSCC, the range ofpedal command is [0,1]
  float x = static_cast<float>(throttle_pedal_percent / 100.0);

  //TODO(xiaochen): Check wether Big Endian or Little Endia
  // The "pedal_command" starts from the 3rd byte of "oscc_brake_command_s"
  memcpy(data+2, &x, sizeof(x));
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
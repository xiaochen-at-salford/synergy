#include "modules/canbus/vehicle/niro/protocol/dec114_hex72_brake_command.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;
 
// const int32_t BrakeCommand_0x72::ID = 0x72;

// public
BrakeCommand_0x72::BrakeCommand_0x72() { Reset(); }

uint32_t BrakeCommand_0x72::GetPeriod() const 
{
  // TODO(All) :  modify every protocol's period manually
  static const uint32_t PERIOD = 20*1000;
  return PERIOD;
}

void BrakeCommand_0x72::UpdateData(uint8_t *data) 
{
  set_p_magic(data);
  set_p_brake_pedal_command(data, brake_pedal_percent_);
}

void BrakeCommand_0x72::Reset() 
{
  // TODO(All) :  you should check this manually
  brake_pedal_percent_ = 0.0;
}

BrakeCommand_0x72 *BrakeCommand_0x72::set_brake_pedal_command(double brake_pedal_percent) 
{
  brake_pedal_percent_ = brake_pedal_percent;
  return this;
}

void BrakeCommand_0x72::set_p_brake_pedal_command(uint8_t *data, double brake_pedal_percent) 
{
  brake_pedal_percent = ProtocolData::BoundedValue(0.0, 100.0, brake_pedal_percent);

  // For OSCC, the range ofpedal command is [0,1]
  float x = static_cast<float>(brake_pedal_percent / 100.0);

  //TODO(xiaochen): Check wether Big Endian or Little Endia
  // The "pedal_command" starts from the 3rd byte of "oscc_brake_command_s"
  memcpy(data+2, &x, sizeof(x));
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
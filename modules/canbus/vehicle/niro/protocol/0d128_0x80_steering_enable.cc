#include "modules/canbus/vehicle/niro/protocol/0d128_0x80_steering_enable.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

// const int32_t BrakeEnable_0x70::ID = 0x70;

// public
SteeringEnable_0x80::SteeringEnable_0x80() { Reset(); }

uint32_t SteeringEnable_0x80::GetPeriod() const 
{
  // TODO(All) :  modify every protocol's period manually
  static const uint32_t PERIOD = 20*1000;
  return PERIOD;
}

void SteeringEnable_0x80::UpdateData(uint8_t *data) 
{
  if (steering_enable_)
  { set_p_magic(data); }
  else
  { data = nullptr; }
}

void SteeringEnable_0x80::Reset() 
{
  // TODO(All) :  you should check this manually
  steering_enable_ = false;
}

SteeringEnable_0x80 *SteeringEnable_0x80::set_brake_enable()
{
  steering_enable_ = true;
  return this;
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
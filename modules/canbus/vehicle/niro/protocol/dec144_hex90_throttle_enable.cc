#include "modules/canbus/vehicle/niro/protocol/dec144_hex90_throttle_enable.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

// const int32_t BrakeEnable_0x70::ID = 0x70;

// public
ThrottleEnable_0x90::ThrottleEnable_0x90() { Reset(); }

uint32_t ThrottleEnable_0x90::GetPeriod() const 
{
  // TODO(All) :  modify every protocol's period manually
  static const uint32_t PERIOD = 20*1000;
  return PERIOD;
}

void ThrottleEnable_0x90::UpdateData(uint8_t *data) 
{
  if (throttle_enable_)
  { set_p_magic(data); }
  else
  { data = nullptr; }
}

void ThrottleEnable_0x90::Reset() 
{
  // TODO(All) :  you should check this manually
  throttle_enable_ = false;
}

ThrottleEnable_0x90 *ThrottleEnable_0x90::set_brake_enable()
{
  throttle_enable_ = true;
  return this;
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
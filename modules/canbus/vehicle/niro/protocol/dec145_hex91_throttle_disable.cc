#include "modules/canbus/vehicle/niro/protocol/dec145_hex91_throttle_disable.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

// public
ThrottleDisable_0x91::ThrottleDisable_0x91() { Reset(); }

uint32_t ThrottleDisable_0x91::GetPeriod() const 
{
  // TODO(All) :  modify every protocol's period manually
  static const uint32_t PERIOD = 20 * 1000;
  return PERIOD;
}

void ThrottleDisable_0x91::UpdateData(uint8_t *data) 
{
  if (throttle_disable_)
  { set_p_magic(data); }
  else
  { data = nullptr; }
}

void ThrottleDisable_0x91::Reset() 
{
  // TODO(All) :  you should check this manually
  throttle_disable_ = false;
}

ThrottleDisable_0x91 *ThrottleDisable_0x91::set_brake_disable()
{
  throttle_disable_ = true;
  return this;
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
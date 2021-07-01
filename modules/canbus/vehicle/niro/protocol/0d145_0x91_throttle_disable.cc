#include "modules/canbus/vehicle/niro/protocol/0d145_0x91_throttle_disable.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

ThrottleDisable_0x91::ThrottleDisable_0x91() 
{ Reset(); }

uint32_t ThrottleDisable_0x91::GetPeriod() 
const {
  static const uint32_t PERIOD = 20*1000;
  return PERIOD;
}

void ThrottleDisable_0x91::UpdateData(uint8_t *data) 
{
  if (!is_active())
  { return; }

  if (use_magic())
  { set_p_magic(data); }
}

void ThrottleDisable_0x91::Reset() 
{ 
  enable_magic_use(); 
  disable_auto_activation();
  deactivate(); 
}

ThrottleDisable_0x91 *ThrottleDisable_0x91::set_throttle_disable()
{
  activate();
  return this;
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
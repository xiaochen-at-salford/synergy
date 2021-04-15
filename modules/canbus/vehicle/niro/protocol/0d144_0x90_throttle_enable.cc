#include "modules/canbus/vehicle/niro/protocol/0d144_0x90_throttle_enable.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

ThrottleEnable_0x90::ThrottleEnable_0x90() 
{
  enable_magic_use();
  disable_auto_activation();
  Reset(); 
}

uint32_t ThrottleEnable_0x90::GetPeriod() 
const {
  static const uint32_t PERIOD = 20*1000;
  return PERIOD;
}

void ThrottleEnable_0x90::UpdateData(uint8_t *data) 
{
  if (!is_active())
  { 
    AERROR << "Attempting to use deactivateed OSCC CAN message"
           << "CAN ID: Ox" << ThrottleEnable_0x90::ID
           << "Check CAN message activation status before calling this function." ;  
  }

  if (use_magic())
  { set_p_magic(data); }

  if (is_auto_active())
  { deactivate(); }
  else
  { activate(); }
}

void ThrottleEnable_0x90::Reset() 
{ deactivate(); }

ThrottleEnable_0x90 *ThrottleEnable_0x90::set_throttle_enable()
{
  activate();
  return this;
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
#include "modules/canbus/vehicle/niro/protocol/0d112_0x70_brake_enable.h"

#include "cyber/common/log.h"
#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

BrakeEnable_0x70::BrakeEnable_0x70() 
{ Reset(); }

uint32_t BrakeEnable_0x70::GetPeriod() const 
{
  static const uint32_t PERIOD = 20*1000;
  return PERIOD;
}

void BrakeEnable_0x70::UpdateData(uint8_t *data) 
{
  if (!is_active())
  {
    return; 
    // AERROR << " Attempting to use deactivateed OSCC CAN message"
    //        << " CAN ID: Ox" << BrakeEnable_0x70::ID
    //        << " Check CAN message activation status before calling this function." ;  
  }

  if (use_magic())
  { set_p_magic(data); }

  if (!is_auto_active())
  { deactivate(); }
}

void BrakeEnable_0x70::Reset() 
{
  enable_magic_use();
  disable_auto_activation();
  deactivate(); 
}

BrakeEnable_0x70 *BrakeEnable_0x70::set_brake_enable()
{
  activate();
  return this;
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
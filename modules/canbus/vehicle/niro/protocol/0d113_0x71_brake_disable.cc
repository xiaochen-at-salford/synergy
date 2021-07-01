#include "modules/canbus/vehicle/niro/protocol/0d113_0x71_brake_disable.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

BrakeDisable_0x71::BrakeDisable_0x71() 
{
  enable_magic_use();
  disable_auto_activation();
  Reset(); 
}

uint32_t BrakeDisable_0x71::GetPeriod() const 
{
  static const uint32_t PERIOD = 20 * 1000;
  return PERIOD;
}

void BrakeDisable_0x71::UpdateData(uint8_t *data) 
{
  if (!is_active())
  { return; }

  if (use_magic())
  { set_p_magic(data); }
}

void BrakeDisable_0x71::Reset() 
{
  deactivate(); 
  disable_auto_activation();
  enable_magic_use();
}

BrakeDisable_0x71 *BrakeDisable_0x71::set_brake_disable()
{
  activate();
  return this;
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
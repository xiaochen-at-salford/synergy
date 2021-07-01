#include "modules/canbus/vehicle/niro/protocol/0d112_0x70_brake_enable.h"

#include "cyber/common/log.h"
#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

BrakeEnable_0x70::BrakeEnable_0x70() { Reset(); }

uint32_t BrakeEnable_0x70::GetPeriod() const 
{
  static const uint32_t PERIOD = 20*1000;
  return PERIOD;
}

void BrakeEnable_0x70::UpdateData(uint8_t *data) 
{
  if (!is_active())
  { return; }

  if (use_magic())
  { set_p_magic(data); }
}

void BrakeEnable_0x70::Reset() 
{
  deactivate(); 
  disable_auto_activation();
  enable_magic_use();
}

BrakeEnable_0x70 *BrakeEnable_0x70::set_brake_enable()
{
  activate();
  return this;
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
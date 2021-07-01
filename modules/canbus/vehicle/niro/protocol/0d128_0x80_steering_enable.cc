#include "modules/canbus/vehicle/niro/protocol/0d128_0x80_steering_enable.h"

namespace apollo {
namespace canbus {
namespace niro {

SteeringEnable_0x80::SteeringEnable_0x80() 
{ Reset(); }

uint32_t SteeringEnable_0x80::GetPeriod() 
const {
  static const uint32_t PERIOD = 20*1000;
  return PERIOD;
}

void SteeringEnable_0x80::UpdateData(uint8_t *data) 
{
  if (!is_active())
  { return; }

  if (use_magic())
  { set_p_magic(data); }
}

void SteeringEnable_0x80::Reset() 
{
  deactivate(); 
  disable_auto_activation();
  enable_magic_use();
}

SteeringEnable_0x80 *SteeringEnable_0x80::set_steering_enable()
{
  activate();
  return this;
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
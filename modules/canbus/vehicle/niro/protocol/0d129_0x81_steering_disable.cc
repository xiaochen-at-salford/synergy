#include "modules/canbus/vehicle/niro/protocol/0d129_0x81_steering_disable.h"

namespace apollo {
namespace canbus {
namespace niro {

SteeringDisable_0x81::SteeringDisable_0x81() 
{ Reset(); }

uint32_t SteeringDisable_0x81::GetPeriod() const 
{
  static const uint32_t PERIOD = 20*1000;
  return PERIOD;
}

void SteeringDisable_0x81::UpdateData(uint8_t *data) 
{
  if (!is_active())
  { return; }

  if (use_magic())
  { set_p_magic(data); }
}

void SteeringDisable_0x81::Reset() 
{ 
  deactivate(); 
  disable_auto_activation();
  enable_magic_use();
}

SteeringDisable_0x81 *SteeringDisable_0x81::SteeringDisable_0x81::set_steering_disable()
{
  activate();
  return this;
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo

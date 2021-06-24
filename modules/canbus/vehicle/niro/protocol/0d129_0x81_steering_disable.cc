#include "modules/canbus/vehicle/niro/protocol/0d129_0x81_steering_disable.h"

namespace apollo {
namespace canbus {
namespace niro {

SteeringDisable_0x81::SteeringDisable_0x81() 
{ 
  enable_magic_use();
  disable_auto_activation();
  Reset(); 
}

uint32_t SteeringDisable_0x81::GetPeriod() const 
{
  static const uint32_t PERIOD = 20*1000;
  return PERIOD;
}

void SteeringDisable_0x81::UpdateData(uint8_t *data) 
{
  if (!is_active())
  {
    return; 
    // AERROR << "Attempting to use deactivateed OSCC CAN message"
    //        << "CAN ID: Ox" << SteeringDisable_0x81::ID
    //        << "Check CAN message activation status before calling this function." ;  
  }

  if (use_magic())
  { set_p_magic(data); }

  if (is_auto_active())
  { activate(); }
  else 
  { deactivate(); }
}

void SteeringDisable_0x81::Reset() 
{ 
  enable_magic_use();
  disable_auto_activation();
  deactivate(); 
}

SteeringDisable_0x81 *SteeringDisable_0x81::SteeringDisable_0x81::set_steering_disable()
{
  activate();
  return this;
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo

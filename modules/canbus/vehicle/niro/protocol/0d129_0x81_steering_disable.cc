#include "modules/canbus/vehicle/niro/protocol/0d129_0x81_steering_disable.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

// const int32_t BrakeDisable_0x71::ID = 0x71;

// public
SteeringDisable_0x81::SteeringDisable_0x81() { Reset(); }

uint32_t SteeringDisable_0x81::GetPeriod() const 
{
  // TODO(All) :  modify every protocol's period manually
  static const uint32_t PERIOD = 20 * 1000;
  return PERIOD;
}

void SteeringDisable_0x81::UpdateData(uint8_t *data) 
{
  if (steering_disable_)
  { set_p_magic(data); }
  else
  { data = nullptr; }
}

void SteeringDisable_0x81::Reset() 
{
  // TODO(All) :  you should check this manually
  steering_disable_ = false;
}

SteeringDisable_0x81 *SteeringDisable_0x81::SteeringDisable_0x81::set_steering_disable()
{
  steering_disable_ = true;
  return this;
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo

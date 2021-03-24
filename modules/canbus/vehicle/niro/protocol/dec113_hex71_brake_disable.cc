#include "modules/canbus/vehicle/niro/protocol/dec113_hex71_brake_disable.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

const int32_t BrakeDisable_0x71::ID = 0x71;

// public
BrakeDisable_0x71::BrakeDisable_0x71() { Reset(); }

uint32_t BrakeDisable_0x71::GetPeriod() const 
{
  // TODO(All) :  modify every protocol's period manually
  static const uint32_t PERIOD = 20 * 1000;
  return PERIOD;
}

void BrakeDisable_0x71::UpdateData(uint8_t* data) 
{
  if (brake_disable_)
  { set_p_magic(data); }
  else
  { data = nullptr; }
}

void BrakeDisable_0x71::Reset() {
  // TODO(All) :  you should check this manually
  brake_disable_ = false;
}

BrakeDisable_0x71* BrakeDisable_0x71::set_brake_disable()
{
  brake_disable_ = true;
  return this;
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
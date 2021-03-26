#include "modules/canbus/vehicle/niro/protocol/dec112_hex70_brake_enable.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

// const int32_t BrakeEnable_0x70::ID = 0x70;

// public
BrakeEnable_0x70::BrakeEnable_0x70() { Reset(); }

uint32_t BrakeEnable_0x70::GetPeriod() const 
{
  // TODO(All) :  modify every protocol's period manually
  static const uint32_t PERIOD = 20*1000;
  return PERIOD;
}

void BrakeEnable_0x70::UpdateData(uint8_t *data) 
{
  if (brake_enable_)
  { set_p_magic(data); }
  else
  { data = nullptr; }
}

void BrakeEnable_0x70::Reset() 
{
  // TODO(All) :  you should check this manually
  brake_enable_ = false;
}

BrakeEnable_0x70 *BrakeEnable_0x70::set_brake_enable()
{
  brake_enable_ = true;
  return this;
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
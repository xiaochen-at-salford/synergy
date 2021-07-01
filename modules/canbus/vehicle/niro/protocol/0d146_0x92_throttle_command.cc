#include "modules/canbus/vehicle/niro/protocol/0d146_0x92_throttle_command.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

ThrottleCommand_0x92::ThrottleCommand_0x92() 
{ 
  enable_magic_use();
  enable_auto_activation();
  activate();
  Reset(); 
}

uint32_t ThrottleCommand_0x92::GetPeriod() const 
{
  static const uint32_t PERIOD = 20*1000; // 50 Hz
  return PERIOD;
}

void ThrottleCommand_0x92::UpdateData(uint8_t *data) 
{
  set_p_magic(data);
  set_p_throttle_pedal_command(data, throttle_pedal_percent_);
}

void ThrottleCommand_0x92::Reset() 
{ 
  activate();
  enable_auto_activation();
  enable_magic_use();
  throttle_pedal_percent_ = 0.0; 
}

ThrottleCommand_0x92 *ThrottleCommand_0x92::set_throttle_pedal_command(double throttle_pedal_percent) 
{
  throttle_pedal_percent_ = throttle_pedal_percent;
  return this;
}

void ThrottleCommand_0x92::set_p_throttle_pedal_command(uint8_t *data, double throttle_pedal_percent) 
{
  // Clean 
  if (throttle_pedal_percent < 0.0)
  { 
    float x = static_cast<float>(0.0);
    std::memcpy(&data[2], &x, sizeof(x));
    return;
  }
  throttle_pedal_percent = ProtocolData::BoundedValue(0.0, 100.0, throttle_pedal_percent);
  // For OSCC, the range of pedal command is [0,1]

  float x = static_cast<float>(throttle_pedal_percent/100.0);
  std::memcpy(&data[2], &x, sizeof(x));
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
#include "modules/canbus/vehicle/niro/protocol/0d114_0x72_brake_command.h"

namespace apollo {
namespace canbus {
namespace niro {

BrakeCommand_0x72::BrakeCommand_0x72() 
{ Reset(); }

uint32_t BrakeCommand_0x72::GetPeriod() const 
{
  static const uint32_t PERIOD = 20*1000;
  return PERIOD;
}

void BrakeCommand_0x72::UpdateData(uint8_t *data) 
{
  if (!is_active())
  { 
    AERROR << "Attempting to use deactivateed OSCC CAN message"
           << "CAN ID: Ox" << BrakeCommand_0x72::ID
           << "Check CAN message activation status before calling this function." ;  
  }

  set_p_magic(data);
  set_p_brake_pedal_command(data, brake_pedal_percent_);

  if (use_magic())
  { set_p_magic(data); }

  if (is_auto_active())
  { activate(); }
  else 
  { deactivate(); }
}

void BrakeCommand_0x72::Reset() 
{ 
  enable_magic_use();
  enable_auto_activation();
  brake_pedal_percent_ = 0.0; 
}

BrakeCommand_0x72 *BrakeCommand_0x72::set_brake_pedal_command(double brake_pedal_percent) 
{
  brake_pedal_percent_ = brake_pedal_percent;
  return this;
}

void BrakeCommand_0x72::set_p_brake_pedal_command(uint8_t *data, double brake_pedal_percent) 
{
  brake_pedal_percent = ProtocolData::BoundedValue(0.0, 100.0, brake_pedal_percent);
  // For OSCC, the range ofpedal command is [0,1]
  float x = static_cast<float>(brake_pedal_percent / 100.0);
  memcpy(&data[2], &x, sizeof(x));
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
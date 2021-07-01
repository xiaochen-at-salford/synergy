#include "modules/canbus/vehicle/niro/protocol/0d114_0x72_brake_command.h"

#include <cmath>

namespace apollo {
namespace canbus {
namespace niro {

BrakeCommand_0x72::BrakeCommand_0x72() 
{
  enable_magic_use();
  enable_auto_activation();
  activate();
  Reset(); 
}

uint32_t BrakeCommand_0x72::GetPeriod() const 
{
  static const uint32_t PERIOD = 20*1000;
  return PERIOD;
}

void BrakeCommand_0x72::UpdateData(uint8_t *data) 
{
  set_p_magic(data);
  set_p_brake_pedal_command(data, brake_pedal_percent_);
}

void BrakeCommand_0x72::Reset() 
{ 
  activate();
  enable_auto_activation();
  enable_magic_use();
  brake_pedal_percent_ = 0.0; 
}

BrakeCommand_0x72 *BrakeCommand_0x72::set_brake_pedal_command(double brake_pedal_percent) 
{
  brake_pedal_percent_ = brake_pedal_percent;
  return this;
}

void BrakeCommand_0x72::set_p_brake_pedal_command(uint8_t *data, double brake_pedal_percent) 
{
  // Clean
  if (brake_pedal_percent < 0)
  {
    float x = static_cast<float>(0.0);
    memcpy(&data[2], &x, sizeof(x));
    return;
  }
  // double scale = 3.5; // medium
  // double scale = 3; // feel good?
  // double scale = 2.5; // not work
  // double offset = 0.0;

  // The nonlinear mapping: ln(a*x+b)
  // | Apollo | OSCC | 
  // | 10%    | 30%  |
  // | 50%    | 90%  |
  brake_pedal_percent /= 100;
  double scale = 0.35491472516316813;
  double offset = 1.3143673350596863;
  brake_pedal_percent = brake_pedal_percent*scale + offset;
  brake_pedal_percent = log(brake_pedal_percent);
  brake_pedal_percent = ProtocolData::BoundedValue(0.0, 1.0, brake_pedal_percent);
  float x = static_cast<float>(brake_pedal_percent);
  // float x = static_cast<float>(0.0);
  memcpy(&data[2], &x, sizeof(x));
  // printf("brake command percent: %f\n", brake_pedal_percent );
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
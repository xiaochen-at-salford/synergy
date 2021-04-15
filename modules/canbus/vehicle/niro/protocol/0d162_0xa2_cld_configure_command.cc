#include "modules/canbus/vehicle/niro/protocol/0d162_0xa2_cld_configure_command.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;
 
// public
CldConfigureCommand_0xA2::CldConfigureCommand_0xA2()
{ Reset(); }

uint32_t CldConfigureCommand_0xA2::GetPeriod() const 
{
  // TODO(All) :  modify every protocol's period manually
  static const uint32_t PERIOD = 20*1000;
  return PERIOD;
}

void CldConfigureCommand_0xA2::UpdateData(uint8_t *data) 
{
  set_p_operational_mode(data, operational_mode_);
  set_p_min_update_interval(data, min_update_interval_);
}

void CldConfigureCommand_0xA2::Reset() 
{
  return;
}

CldConfigureCommand_0xA2 *CldConfigureCommand_0xA2::set_operational_mode(OperationalModeEnum operational_mode) 
{
  operational_mode_ = operational_mode;
  return this;
}

CldConfigureCommand_0xA2 *CldConfigureCommand_0xA2::set_min_update_interval(double min_update_interval) 
{
  min_update_interval_ = min_update_interval;
  return this;
}

void CldConfigureCommand_0xA2::set_p_operational_mode(uint8_t *data, OperationalModeEnum min_update_interval) 
{
  //TODO(xiaochen)
  return;
}

void CldConfigureCommand_0xA2::set_p_min_update_interval(uint8_t *data, double min_update_interval) 
{
  //TODOxiaochen)
  return;
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
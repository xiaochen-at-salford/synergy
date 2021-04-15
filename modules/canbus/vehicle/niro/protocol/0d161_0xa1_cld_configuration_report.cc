#include "modules/canbus/vehicle/niro/protocol/0d161_0xa1_cld_configuration_report.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

CldConfigurationReport_0xA1::CldConfigurationReport_0xA1() {};

void CldConfigurationReport_0xA1::Parse(const std::uint8_t *bytes, int32_t length, ChassisDetail *chassis) 
const {
  chassis->mutable_niro()->mutable_cld_configuration_report_0xa1(
      )->set_operational_mode(operational_mode(bytes, length)) ;

  chassis->mutable_niro()->mutable_cld_configuration_report_0xa1(
      )->set_min_update_interval(min_update_interval(bytes, length)) ;
}

OperationModeEnum CldConfigurationReport_0xA1::operational_mode(const std::uint8_t *bytes, int32_t length) 
const {
  OperationModeEnum x = OperationModeEnum::Cld_Configuration_Report_0xA1_OperationalMode_Enum_DISABLED;
  return x;
}

double CldConfigurationReport_0xA1::min_update_interval(const std::uint8_t *bytes, int32_t length) 
const {
  double x = 0.0;
  return x;
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
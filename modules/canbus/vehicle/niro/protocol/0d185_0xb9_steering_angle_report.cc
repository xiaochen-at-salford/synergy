#include "modules/canbus/vehicle/niro/protocol/0d185_0xb9_steering_angle_report.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

SteeringAngleReport_0xB9::SteeringAngleReport_0xB9() {};

// const int32_t BrakeReport_0x73::ID = 0x73;

void SteeringAngleReport_0xB9::Parse(const std::uint8_t *bytes, int32_t length, ChassisDetail *chassis) 
const {
  chassis->mutable_niro()->mutable_steering_angle_report(
      )->set_steering_angle_report_flags(steering_angle_report_flags(bytes, length));

  chassis->mutable_niro()->mutable_steering_angle_report(
      )->set_steering_angle_report_errors(steering_angle_report_errors(bytes, length));
}

SteeringAngleReportFlagsEnum SteeringAngleReport_0xB9::steering_angle_report_flags(const std::uint8_t *bytes, int32_t length) 
const {
  Byte t(bytes+0);
  int8_t x = t.get_byte(0, 1);
  return static_cast<SteeringAngleReportFlagsEnum>(x);
}

SteeringAngleReportErrorsEnum SteeringAngleReport_0xB9::steering_angle_report_errors(const std::uint8_t *bytes, int32_t length) 
const {
  Byte t(bytes+1);
  int8_t x = t.get_byte(0, 1);
  return static_cast<SteeringAngleReportErrorsEnum>(x);
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
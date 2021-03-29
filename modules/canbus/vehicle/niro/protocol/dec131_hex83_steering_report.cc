#include "modules/canbus/vehicle/niro/protocol/dec131_hex83_steering_report.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

SteeringReport_0x83::SteeringReport_0x83() {};

// const int32_t BrakeReport_0x73::ID = 0x73;

void SteeringReport_0x83::Parse(const std::uint8_t *bytes, int32_t length, ChassisDetail *chassis) 
const {
  chassis->mutable_niro()->mutable_steering_report_0x83(
      )->set_steering_report_enabled(steering_report_enabled(bytes, length));

  chassis->mutable_niro()->mutable_steering_report_0x83(
      )->set_steering_operator_override(steering_operator_override(bytes, length));

  chassis->mutable_niro()->mutable_steering_report_0x83(
      )->set_steering_dtcs(steering_dtcs(bytes, length));
}

bool SteeringReport_0x83::steering_report_enabled(const std::uint8_t *bytes, int32_t length) 
const {
  Byte t(bytes+2);
  int8_t x = t.get_byte(0, 1);
  return (x == 1);
}

bool SteeringReport_0x83::steering_operator_override(const std::uint8_t *bytes, int32_t length) 
const {
  Byte t(bytes+3);
  int8_t x = t.get_byte(0, 1);
  return (x == 1);
}

Steering_Report_0x83::SteeringDtcs::Enum SteeringReport_0x83::steering_dtcs(const std::uint8_t *bytes, int32_t length) 
const {
  Byte t(bytes+4);
  int8_t x = t.get_byte(0, 1);
  //TODO(xiaochen): create this proto enum
  return static_cast<Steering_Report_0x83::SteeringDtcs::Enum>(x);
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
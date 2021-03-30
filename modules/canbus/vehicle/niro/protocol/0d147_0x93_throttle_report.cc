#include "modules/canbus/vehicle/niro/protocol/0d147_0x93_throttle_report.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

ThrottleReport_0x93::ThrottleReport_0x93() {};

void ThrottleReport_0x93::Parse(const std::uint8_t *bytes, int32_t length, ChassisDetail *chassis) 
const {
  chassis->mutable_niro()->mutable_throttle_report_0x93(
      )->set_throttle_report_enabled(throttle_report_enabled(bytes, length));

  chassis->mutable_niro()->mutable_throttle_report_0x93(
      )->set_throttle_operator_override(throttle_operator_override(bytes, length));

  chassis->mutable_niro()->mutable_throttle_report_0x93(
      )->set_throttle_dtcs(throttle_dtcs(bytes, length));
}

bool ThrottleReport_0x93::throttle_report_enabled(const std::uint8_t *bytes, int32_t length) 
const {
  Byte t(bytes+2);
  int8_t x = t.get_byte(0, 1);
  return (x == 1);
}

bool ThrottleReport_0x93::throttle_operator_override(const std::uint8_t *bytes, int32_t length) 
const {
  Byte t(bytes+3);
  int8_t x = t.get_byte(0, 1);
  return (x == 1);
}

ThrottleDtcsEnum ThrottleReport_0x93::throttle_dtcs(const std::uint8_t *bytes, int32_t length) 
const {
  Byte t(bytes+4);
  int8_t x = t.get_byte(0, 1);
  //TODO(xiaochen): create this proto enum
  return static_cast<ThrottleDtcsEnum>(x);
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
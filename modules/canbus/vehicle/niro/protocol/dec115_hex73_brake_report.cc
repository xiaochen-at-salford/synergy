#include "modules/canbus/vehicle/niro/protocol/dec115_hex73_brake_report.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

BrakeReport_0x73::BrakeReport_0x73() {};

// const int32_t BrakeReport_0x73::ID = 0x73;

void BrakeReport_0x73::Parse(const std::uint8_t *bytes, int32_t length, ChassisDetail *chassis) 
const {
  chassis->mutable_niro()->mutable_brake_report_0x73(
      )->set_brake_report_enabled(brake_report_enabled(bytes, length));

  chassis->mutable_niro()->mutable_brake_report_0x73(
      )->set_brake_operator_override(brake_operator_override(bytes, length));

  chassis->mutable_niro()->mutable_brake_report_0x73(
      )->set_brake_dtcs(brake_dtcs(bytes, length));
}

bool BrakeReport_0x73::brake_report_enabled(const std::uint8_t *bytes, int32_t length) 
const {
  Byte t(bytes+2);
  int8_t x = t.get_byte(0, 1);
  return (x == 1);
}

bool BrakeReport_0x73::brake_operator_override(const std::uint8_t *bytes, int32_t length) 
const {
  Byte t(bytes+3);
  int8_t x = t.get_byte(0, 1);
  return (x == 1);
}

Brake_Report_0x73::BrakeDtcs::Enum BrakeReport_0x73::brake_dtcs(const std::uint8_t *bytes, int32_t length) 
const {
  Byte t(bytes+4);
  int8_t x = t.get_byte(0, 1);
  //TODO(xiaochen): create this proto enum
  return static_cast<Brake_Report_0x73::BrakeDtcs::Enum>(x);
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
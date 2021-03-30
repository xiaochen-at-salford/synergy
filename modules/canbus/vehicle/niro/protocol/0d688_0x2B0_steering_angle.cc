#include "modules/canbus/vehicle/niro/protocol/dec688_hex2B0_steering_angle.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

SteeringAngle_0x2B0::SteeringAngle_0x2B0() {};

void SteeringAngle_0x2B0::Parse(const std::uint8_t *bytes, int32_t length, ChassisDetail *chassis) 
const {
  chassis->mutable_niro()->mutable_brake_report_0x73(
      )->set_steering_angle(steering_angle(bytes, length));

}

double SteeringAngle_0x2B0::steering_angle(const std::uint8_t *bytes, int32_t length) 
const {
  Byte t(bytes);
  int8_t x0 = t.get_byte(0, 1);
  int8_t x1 = t.get_byte(1, 1);
  return -static_cast<double>((x1<<8) || x0);
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
#include "modules/canbus/vehicle/niro/protocol/0d688_0x2B0_steering_angle.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

SteeringAngle_0x2B0::SteeringAngle_0x2B0() {};

void SteeringAngle_0x2B0::Parse(const std::uint8_t *bytes, int32_t length, ChassisDetail *chassis_detail) 
const {
  chassis_detail->mutable_niro()
      ->mutable_steering_angle()
      ->set_steering_angle(steering_angle(bytes, length)) ;

  // Used for debug  
  // chassis_detail->mutable_niro()
  //     ->set_find_steering_angle_msg(true);
}

//wip...
double SteeringAngle_0x2B0::steering_angle(const std::uint8_t *bytes, int32_t length) 
const {
  int16_t raw = (bytes[1] << 8) | bytes[0];
  return -((double)raw * 0.1);

  // Byte t(bytes);
  // int8_t x0 = t.get_byte(0, 1);
  // int8_t x1 = t.get_byte(1, 1);
  // return -static_cast<double>((x1<<8) || x0);
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
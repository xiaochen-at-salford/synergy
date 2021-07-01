#include "modules/canbus/vehicle/niro/protocol/0d902_0x386_wheel_speed.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

// const int32_t BrakePressure_0x220::ID = 0x220;

// public
WheelSpeed_0x386::WheelSpeed_0x386() {};

void WheelSpeed_0x386::Parse(const std::uint8_t *bytes, int32_t length, ChassisDetail *chassis_detail) 
const {
  double scale = 1/1.609344; // kph -> mph
  chassis_detail->mutable_niro()
      ->mutable_wheel_speeds()
      ->set_left_rear(wheel_speed_left_rear(bytes, length)*scale) ;

  chassis_detail->mutable_niro()
      ->mutable_wheel_speeds()
      ->set_right_rear(wheel_speed_right_rear(bytes, length)*scale) ;

  chassis_detail->mutable_niro()
      ->mutable_wheel_speeds()
      ->set_left_front(wheel_speed_left_front(bytes, length)*scale) ;

  chassis_detail->mutable_niro()
      ->mutable_wheel_speeds()
      ->set_right_front(wheel_speed_right_front(bytes, length)*scale) ;

  // Used for debug
  // chassis_detail->mutable_niro()
  //     ->set_find_wheel_speed_msg(true);
}

//TODO(xiaochen): Correct the decoding part
double WheelSpeed_0x386::wheel_speed(const std::uint8_t *bytes, const size_t offset) 
const {
  uint16_t raw = ((bytes[offset + 1] & 0x0F) << 8) | bytes[offset];
  // 10^-1 precision, raw / 32.0
  return (double)((int)((double)raw / 3.2) / 10.0);
  // Byte t(bytes+4);
  // int16_t x = ((t.get_byte(1, 1) & 0x0F) << 8) | t.get_byte(0, 1);
  // double scale = 40.0;
  // return static_cast<double>(x/scale);
}

double WheelSpeed_0x386::wheel_speed_right_rear(const std::uint8_t *bytes, int32_t length) 
const {
  size_t offset = 6;
  return wheel_speed(bytes, offset);
  // Byte t(bytes+4);
  // int16_t x = ((t.get_byte(1, 1) & 0x0F) << 8) | t.get_byte(0, 1);
  // double scale = 40.0;
  // return static_cast<double>(x/scale);
}

double WheelSpeed_0x386::wheel_speed_left_rear(const std::uint8_t *bytes, int32_t length) 
const {
  size_t offset = 4;
  return wheel_speed(bytes, offset);
  // Byte t(bytes+4);
  // int16_t x = ((t.get_byte(1, 1) & 0x0F) << 8) | t.get_byte(0, 1);
  // double scale = 40.0;
  // return static_cast<double>(x/scale);
}

double WheelSpeed_0x386::wheel_speed_right_front(const std::uint8_t *bytes, int32_t length) 
const {
  size_t offset = 2;
  return wheel_speed(bytes, offset);
  // Byte t(bytes+4);
  // int16_t x = ((t.get_byte(1, 1) & 0x0F) << 8) | t.get_byte(0, 1);
  // double scale = 40.0;
  // return static_cast<double>(x/scale);
}

double WheelSpeed_0x386::wheel_speed_left_front(const std::uint8_t *bytes, int32_t length) 
const {
  size_t offset = 0;
  return wheel_speed(bytes, offset);
  // Byte t(bytes+4);
  // int16_t x = ((t.get_byte(1, 1) & 0x0F) << 8) | t.get_byte(0, 1);
  // double scale = 40.0;
  // return static_cast<double>(x/scale);
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
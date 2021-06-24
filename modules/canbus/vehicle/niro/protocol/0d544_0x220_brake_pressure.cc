#include "modules/canbus/vehicle/niro/protocol/0d544_0x220_brake_pressure.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

// const int32_t BrakePressure_0x220::ID = 0x220;

BrakePressure_0x220::BrakePressure_0x220() {};

void BrakePressure_0x220::Parse(const std::uint8_t *bytes, int32_t length, ChassisDetail *chassis_detail) 
const {
  chassis_detail->mutable_niro()
      ->mutable_brake_pressure()
      ->set_brake_pressure(brake_pressure(bytes, length)) ;

  // Used for debug
  // chassis_detail->mutable_niro()
  //     ->set_dummy_variable_in_msg(true);
}

double BrakePressure_0x220::brake_pressure(const std::uint8_t *bytes, int32_t length) 
const {
  double scale = 40.0;
  uint16_t raw = ((bytes[4] & 0x0F) << 8) | bytes[3];
  return (double)raw/scale;
  // Byte t(bytes+4);
  // int16_t x = ((t.get_byte(1, 1) & 0x0F) << 8) | t.get_byte(0, 1);
  // double scale = 40.0;
  // return static_cast<double>(x/scale);
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
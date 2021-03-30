#include "modules/canbus/vehicle/niro/protocol/dec544_hex220_brake_pressure.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

const int32_t BrakePressure_0x220::ID = 0x220;

// public
BrakePressure_0x220::BrakePressure_0x220() { Reset(); }

void BrakePressure_0x220::Parse(const std::uint8_t *bytes, int32_t length, ChassisDetail *chassis) 
const {
  chassis->mutable_niro()->mutable_brake_pressure_0x220(
      )->set_brake_pressure(brake_pressure(bytes, length));
}

double BrakePressure_0x220::brake_pressure(const std::uint8_t *bytes, int32_t length) 
const {
  Byte t(bytes+4);
  int16_t x = ((t.get_byte(1, 1) & 0x0F) << 8) | t.get_byte(0, 1);
  double scale = 40.0;
  return static_cast<double>{x/sacle};

}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
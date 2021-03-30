#include "modules/canbus/vehicle/niro/protocol/dec160_hexA0_cld_report.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

CldReport_0xA0::CldReport_0xA0() {};

void CldReport_0xA0::Parse(const std::uint8_t *bytes, int32_t length, ChassisDetail *chassis) 
const {
  chassis->mutable_niro()->mutable_cld_report_0xa0(
      )->set_state(state(bytes, length)) ;

  chassis->mutable_niro()->mutable_cld_report_0xa0(
      )->set_last_disable_reason(last_disable_reason(bytes, length)) ;

  chassis->mutable_niro()->mutable_cld_report_0xa0(
      )->set_last_transition_ago(last_transition_ago(bytes, length)) ;

  chassis->mutable_niro()->mutable_cld_report_0xa0(
      )->set_bus_frame_rate(bus_frame_rate(bytes, length)) ;

  chassis->mutable_niro()->mutable_cld_report_0xa0(
      )->set_bus_frame_rate(bus_frame_rate(bytes, length)) ;

  chassis->mutable_niro()->mutable_cld_report_0xa0(
      )->set_bus_error_count(bus_error_count(bytes, length)) ;

  chassis->mutable_niro()->mutable_cld_report_0xa0(
      )->set_frame_rate_steering(frame_rate_steering(bytes, length)) ;

  chassis->mutable_niro()->mutable_cld_report_0xa0(
      )->set_frame_rate_throttle(frame_rate_throttle(bytes, length)) ;

  chassis->mutable_niro()->mutable_cld_report_0xa0(
      )->set_frame_rate_brake(frame_rate_brake(bytes, length)) ;
  
}

StateEnum CldReport_0xA0::state(const std::uint8_t *bytes, int32_t length) 
const {
  Byte t(bytes+0);
  int8_t x = t.get_byte(0, 1) & 0b00000111;
  return static_cast<StateEnum>(x);
}

LastDisableReasonEnum CldReport_0xA0::last_disable_reason(const std::uint8_t *bytes, int32_t length) 
const {
  Byte t(bytes+0);
  int8_t x = t.get_byte(0, 1) & 0b00111000;
  return static_cast<LastDisableReasonEnum>(x);
}

uint32_t CldReport_0xA0::last_transition_ago(const std::uint8_t *bytes, int32_t length) 
const {
  uint16_t x = 0;
  std:memcpy(&x, bytes, 2); 
  return static_cast<uint16_t>(x);
}

uint32_t CldReport_0xA0::bus_frame_rate(const std::uint8_t *bytes, int32_t length) 
const {
  uint16_t x = 0;
  return x;
}

uint32_t CldReport_0xA0::bus_error_count(const std::uint8_t *bytes, int32_t length) 
const {
  uint16_t x = 0;
  return x;
}

uint32_t CldReport_0xA0::frame_rate_steering(const std::uint8_t *bytes, int32_t length) 
const {
  uint16_t x = 0;
  return x;
}

uint32_t CldReport_0xA0::frame_rate_throttle(const std::uint8_t *bytes, int32_t length) 
const {
  uint16_t x = 0;
  return x;
}

uint32_t CldReport_0xA0::frame_rate_brake(const std::uint8_t *bytes, int32_t length) 
const {
  uint16_t x = 0;
  return x;
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
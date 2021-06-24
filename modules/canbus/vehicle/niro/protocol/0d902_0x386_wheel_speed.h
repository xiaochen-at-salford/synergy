#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

class WheelSpeed_0x386 : public OsccProtocolBase {
 public:
  static const int32_t ID = 0x386;

  WheelSpeed_0x386();

  void Parse(const std::uint8_t *bytes, int32_t length, ChassisDetail *chassis) const override;

 private:
  //TODO(xiaochen): Correct the reading
  double wheel_speed(const std::uint8_t *bytes, const size_t offset) const;
  double wheel_speed_right_rear(const std::uint8_t *bytes, int32_t length) const;
  double wheel_speed_left_rear(const std::uint8_t *bytes, int32_t length) const;
  double wheel_speed_right_front(const std::uint8_t *bytes, int32_t length) const;
  double wheel_speed_left_front(const std::uint8_t *bytes, int32_t length) const;
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
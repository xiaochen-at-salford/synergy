#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

class BrakePressure_0x220 : public OsccProtocolBase {
 public:
  static const int32_t ID = 0x220;

  BrakePressure_0x220();

  void Parse(const std::uint8_t *bytes, int32_t length, ChassisDetail *chassis) const override;

 private:
  //TODO(xiaochen): Correct the reading
  double brake_pressure(const std::uint8_t *bytes, int32_t length) const;
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
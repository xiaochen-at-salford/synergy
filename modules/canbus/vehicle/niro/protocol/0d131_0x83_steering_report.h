#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

typedef ChannelReport::Type ChannelReportEnum;

class SteeringReport_0x83 : public OsccProtocolBase {

 public:
  static const int32_t ID = 0x73;

  SteeringReport_0x83();

  void Parse(const std::uint8_t *bytes, int32_t length, ChassisDetail *chassis) const override;

 private:
  bool steering_report_enabled(const std::uint8_t *bytes, int32_t length) const;

  bool steering_operator_override(const std::uint8_t *bytes, int32_t length) const;

  ChannelReportEnum steering_dtcs(const std::uint8_t *bytes, int32_t length) const;
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
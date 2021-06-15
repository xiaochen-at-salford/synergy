#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

typedef ChannelReport::Type ChannelReportEnum;

class ThrottleReport_0x93 
  : public ::apollo::drivers::canbus::ProtocolData<::apollo::canbus::ChassisDetail> {

 public:
  static const int32_t ID = 0x73;

  ThrottleReport_0x93();

  void Parse(const std::uint8_t *bytes, int32_t length, ChassisDetail *chassis) const override;

 private:
  bool throttle_report_enabled(const std::uint8_t *bytes, int32_t length) const;

  bool throttle_operator_override(const std::uint8_t *bytes, int32_t length) const;

  //TODO(xiaochen):
  ChannelReportEnum throttle_dtcs(const std::uint8_t *bytes, int32_t length) const;
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
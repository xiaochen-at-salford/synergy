#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

typedef Cld_Report_0xA0::State::Enum StateEnum;

typedef Cld_Report_0xA0::LastDisableReason::Enum  LastDisableReasonEnum;

class CldReport_0xA0 
  : public OsccProtocolBase,
    public ::apollo::drivers::canbus::ProtocolData<::apollo::canbus::ChassisDetail> {

 public:
  static const int32_t ID = 0xA0;

  CldReport_0xA0();

  void Parse(const std::uint8_t *bytes, int32_t length, ChassisDetail *chassis) const override;

 private:
  StateEnum state(const std::uint8_t *bytes, int32_t length) const;

  LastDisableReasonEnum last_disable_reason(const std::uint8_t *bytes, int32_t length) const;

  uint32_t last_transition_ago(const std::uint8_t *bytes, int32_t length) const;

  uint32_t bus_frame_rate(const std::uint8_t *bytes, int32_t length) const;

  //TODO(xiaochen):
  uint32_t bus_error_count(const std::uint8_t *bytes, int32_t length) const;

  //TODO(xiaochen):
  uint32_t frame_rate_steering(const std::uint8_t *bytes, int32_t length) const;

  //TODO(xiaochen):
  uint32_t frame_rate_throttle(const std::uint8_t *bytes, int32_t length) const;

  //TODO(xiaochen):
  uint32_t frame_rate_brake(const std::uint8_t *bytes, int32_t length) const;
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
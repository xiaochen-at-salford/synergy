#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

typedef FaultOriginId::Type FaultOriginIdEnum

typedef ChannelReport::Type ChannelReportEnum;

class FaultReport_0xAF : public OsccProtocolBase {

 public:
  static const int32_t ID = 0xA0;

  FaultReport_0xAF();

  void Parse(const std::uint8_t *bytes, int32_t length, ChassisDetail *chassis) const override;

 private:
  FaultOriginIdEnum fault_origin_id(const std::uint8_t *bytes, int32_t length) const;

  FaultDtcsEnum fault_dtcs(const std::uint8_t *bytes, int32_t length) const;
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

typedef Cld_Configuration_Report_0xA1::OperationalMode::Enum OperationModeEnum;

typedef Cld_Report_0xA0::LastDisableReason::Enum  LastDisableReasonEnum;

class CldConfigurationReport_0xA1 
  : public OsccProtocolBase,
    public ::apollo::drivers::canbus::ProtocolData<::apollo::canbus::ChassisDetail> {

 public:
  static const int32_t ID = 0xA0;

  CldConfigurationReport_0xA1();

  void Parse(const std::uint8_t *bytes, int32_t length, ChassisDetail *chassis) const override;

 private:
  //TODO(xiaochen):
  OperationModeEnum operational_mode(const std::uint8_t *bytes, int32_t length) const;

  // TODO(xiaochen):
  double min_update_interval(const std::uint8_t *bytes, int32_t length) const;
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
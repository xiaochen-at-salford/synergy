#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

typedef Steering_Angle_Report_0xB9::SteeringAngleReportFlags::Type SteeringAngleReportFlagsEnum;
typedef Steering_Angle_Report_0xB9::SteeringAngleReportErrors::Type SteeringAngleReportErrorsEnum;

class SteeringAngleReport_0xB9
  : public ::apollo::drivers::canbus::ProtocolData<::apollo::canbus::ChassisDetail> {

 public:
  static const int32_t ID = 0x73;

  SteeringAngleReport_0xB9();

  void Parse(const std::uint8_t *bytes, int32_t length, ChassisDetail *chassis) const override;

 private:
  SteeringAngleReportFlagsEnum steering_angle_report_flags(const std::uint8_t *bytes, int32_t length) const;

  SteeringAngleReportErrorsEnum steering_angle_report_errors(const std::uint8_t *bytes, int32_t length) const;
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
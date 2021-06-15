#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

/* From the KIA Niro DBC file:
 *
 * BO_ 115 BRAKE_REPORT: 8 BRAKE 
 *    SG_ brake_report_magic : 0|16@1+ (1,0) [0|0] "" BRAKE
 *    SG_ brake_report_enabled : 16|8@1+ (1,0) [0|0] "" BRAKE
 *    SG_ brake_operator_override : 24|8@1+ (1,0) [0|0] "" BRAKE
 *    SG_ brake_dtcs : 32|8@1+ (1,0) [0|0] "" BRAK
 * 
 * VAL_ 115 brake_report_magic 1372 "Default Magic Number";
 * VAL_ 115 brake_dtcs 0 "No communication with brake sensor" 1 "Operator Override: Brake";
 * VAL_ 115 brake_operator_override 0 "Not overridden by operator." 1 "Control overridden by operator.";
 */

typedef ChannelReport::Type ChannelReportEnum;

class BrakeReport_0x73 : public OsccProtocolBase {
 public:
  static const int32_t ID = 0x73;

  BrakeReport_0x73();

  void Parse(const std::uint8_t *bytes, int32_t length, ChassisDetail *chassis) const override;

 private:
  bool brake_report_enabled(const std::uint8_t *bytes, int32_t length) const;

  bool brake_operator_override(const std::uint8_t *bytes, int32_t length) const;

  //TODO(xiaochen):
  ChannelReportEnum brake_dtcs(const std::uint8_t *bytes, int32_t length) const;
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
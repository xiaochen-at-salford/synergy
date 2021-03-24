#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

/* From OSCC SDK:
 *
 * typedef struct {
 *   uint8_t magic[2]; // Magic number identifying CAN frame as from OSCC.
 *                     // Byte 0 should be \ref OSCC_MAGIC_BYTE_0.
 *                     // Byte 1 should be \ref OSCC_MAGIC_BYTE_1.
 *   uint8_t enabled; // Braking controls enabled state.
 *                    // Zero value means disabled (commands are ignored).
 *                    // Non-zero value means enabled (no timeouts or overrides have occured).
 *   uint8_t operator_override; // Driver override state.
 *                              // Zero value means there has been no operator override.
 *                              // Non-zero value means an operator has physically overridden
 *                              // the system.
 *   uint8_t dtcs; // Bitfield of DTCs present in the module.
 *   uint8_t reserved[3]; // Reserved.
 * } oscc_brake_report_s;
 */

/* From PolySync Devkit KIA Niro DBC:
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

class BrakeReport_0x73 
  : public OsccProtocolBase,
    public ::apollo::drivers::canbus::ProtocolData<::apollo::canbus::ChassisDetail> {

 public:
  static const int32_t ID;

  BrakeReport_0x73();

  void Parse(const std::uint8_t *bytes, int32_t length, ChassisDetail *chassis) const override;

 private:
  bool brake_report_enabled(const std::uint8_t *bytes, int32_t length) const;

  bool brake_operator_override(const std::uint8_t *bytes, int32_t length) const;

  //TODO(xiaochen):
  Brake_Report_0x73::Brake_Dtc_Type brake_dtc(const std::uint8_t *bytes, int32_t length) const;
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
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
 *                     /brake_command_0x72_/ Byte 1 should be \ref OSCC_MAGIC_BYTE_1.
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

typedef ChannelReport::Type ChannelReportEnum;

class SteeringReport_0x83 : public OsccProtocolBase {

 public:
  static const int32_t ID = 0x83;

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
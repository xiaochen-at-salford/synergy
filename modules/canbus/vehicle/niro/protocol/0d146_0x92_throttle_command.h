#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

/* From OSCC SDK:
 * typedef struct {
 *   uint8_t magic[2]; // Magic number identifying CAN frame as from OSCC.
 *                     // Byte 0 should be \ref OSCC_MAGIC_BYTE_0.
 *                     // Byte 1 should be \ref OSCC_MAGIC_BYTE_1.
 *   float pedal_command; // Brake Request 0.0 to 1.0 where 1.0 is 100% 
 *   uint8_t reserved[2]; // Reserved. 
 * } oscc_brake_command_s;
 */

class ThrottleCommand_0x92 
  : public OsccProtocolBase,
    public ::apollo::drivers::canbus::ProtocolData<::apollo::canbus::ChassisDetail> {
 public:
  static const int32_t ID = 0x92;

  ThrottleCommand_0x92();

  uint32_t GetPeriod() const override;

  void UpdateData(uint8_t* data) override;

  void Reset() override;

  /**
   * @brief Set the brake pedal command
   * 
   * @param throttle_pedal_position The percentage of pedal position [0~100]
   * @return BrakeCommand_0x72* 
   */
  ThrottleCommand_0x92* set_throttle_pedal_command(double throttle_pedal_position);

 private:

  // Frome OSCC DBC file
  // BO_ 114 BRAKE_COMMAND: 8 BRAKE
  //    SG_ brake_command_magic : 0|16@1+ (1,0) [0|0] "" BRAKE
  //    SG_ brake_pedal_command : 16|32@1+ (1,0) [0|1] "percent" BRAKE
  void set_p_throttle_pedal_command(uint8_t *data, double throttle_pedal_percent);

 private:
  double throttle_pedal_percent_;
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo

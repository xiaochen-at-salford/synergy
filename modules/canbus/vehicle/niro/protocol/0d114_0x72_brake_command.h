#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

/* From the KIA Niro DBC file:
 * BO_ 114 BRAKE_COMMAND: 8 BRAKE
 *    SG_ brake_command_magic : 0|16@1+ (1,0) [0|0] "" BRAKE
 *    SG_ brake_pedal_command : 16|32@1+ (1,0) [0|1] "percent" BRAKE
 */

class BrakeCommand_0x72 : public OsccProtocolBase {
 public:
  static const int32_t ID = 0x72;

  BrakeCommand_0x72();

  uint32_t GetPeriod() const override;

  void UpdateData(uint8_t* data) override;

  void Reset() override;

  BrakeCommand_0x72* set_brake_pedal_command(double brake_pedal_percent);

 private:
  void set_p_brake_pedal_command(uint8_t *data, double brake_pedal_percent);

 private:
  double brake_pedal_percent_; // [%]:[0.0, 100.0]
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo

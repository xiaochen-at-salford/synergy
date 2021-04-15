#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

typedef Cld_Configure_Command_0xA2::OperationalMode::Enum OperationalModeEnum;

class CldConfigureCommand_0xA2 
  : public OsccProtocolBase,
    public ::apollo::drivers::canbus::ProtocolData<::apollo::canbus::ChassisDetail> {
 public:
  static const int32_t ID = 0xA2;

  CldConfigureCommand_0xA2();

  uint32_t GetPeriod() const override;

  void UpdateData(uint8_t* data) override;

  void Reset() override;

  CldConfigureCommand_0xA2 *set_operational_mode(OperationalModeEnum operational_modoe);

  CldConfigureCommand_0xA2 *set_min_update_interval(double min_update_interval);

 private:
  void set_p_operational_mode(uint8_t *data, OperationalModeEnum operaional_modoe);

  void set_p_min_update_interval(uint8_t *data, double min_update_interval);

 private:
    OperationalModeEnum operational_mode_;
    double min_update_interval_;
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo


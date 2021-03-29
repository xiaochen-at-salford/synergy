#include "modules/canbus/vehicle/niro/niro_message_manager.h"

#include "modules/canbus/vehicle/niro/protocol/dec112_hex70_brake_enable.h"
#include "modules/canbus/vehicle/niro/protocol/dec113_hex71_brake_disable.h"
#include "modules/canbus/vehicle/niro/protocol/dec114_hex72_brake_command.h"
#include "modules/canbus/vehicle/niro/protocol/dec115_hex73_brake_report.h"

#include "modules/canbus/vehicle/niro/protocol/dec128_hex80_steering_enable.h"
#include "modules/canbus/vehicle/niro/protocol/dec129_hex81_steering_disable.h"
#include "modules/canbus/vehicle/niro/protocol/dec130_hex82_steering_torque_command.h"
#include "modules/canbus/vehicle/niro/protocol/dec131_hex83_steering_report.h"

#include "modules/canbus/vehicle/niro/protocol/dec184_hexB8_steering_angle_command.h"
#include "modules/canbus/vehicle/niro/protocol/dec185_hexB9_steering_angle_report.h"

// #include "modules/canbus/vehicle/niro/protocol/dec544_hex220_brake_pressure.h"

namespace apollo {
namespace canbus {
namespace niro {

NiroMessageManager::NiroMessageManager() 
{
  #define NEED_CHECK true

  // Control Messages
  AddSendProtocolData<BrakeEnable_0x70, NEED_CHECK>();
  AddSendProtocolData<BrakeDisable_0x71, NEED_CHECK>();
  AddSendProtocolData<BrakeCommand_0x72, NEED_CHECK>();

  AddSendProtocolData<SteeringEnable_0x80, NEED_CHECK>();
  AddSendProtocolData<SteeringDisable_0x81, NEED_CHECK>();
  AddSendProtocolData<SteeringTorqueCommand_0x82, NEED_CHECK>();
  AddSendProtocolData<SteeringAngleCommand_0xB8, NEED_CHECK>();

  // Report Messages
  AddRecvProtocolData<BrakeReport_0x73, NEED_CHECK>();

  AddSendProtocolData<SteeringReport_0x83, NEED_CHECK>();
  AddSendProtocolData<SteeringAngleReport_0xB9, NEED_CHECK>();

  // AddRecvProtocolData<BrakeReport_0x220, need_check>();
}

NiroMessageManager::~NiroMessageManager() {}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo

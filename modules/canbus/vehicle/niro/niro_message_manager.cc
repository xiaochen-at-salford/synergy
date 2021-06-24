#include "modules/canbus/vehicle/niro/niro_message_manager.h"
// Brake protocols
#include "modules/canbus/vehicle/niro/protocol/0d112_0x70_brake_enable.h"
#include "modules/canbus/vehicle/niro/protocol/0d113_0x71_brake_disable.h"
#include "modules/canbus/vehicle/niro/protocol/0d114_0x72_brake_command.h"
#include "modules/canbus/vehicle/niro/protocol/0d115_0x73_brake_report.h"

// Steering protocols
#include "modules/canbus/vehicle/niro/protocol/0d128_0x80_steering_enable.h"
#include "modules/canbus/vehicle/niro/protocol/0d129_0x81_steering_disable.h"
#include "modules/canbus/vehicle/niro/protocol/0d130_0x82_steering_torque_command.h"
#include "modules/canbus/vehicle/niro/protocol/0d131_0x83_steering_report.h"
#include "modules/canbus/vehicle/niro/protocol/0d184_0xb8_steering_angle_command.h"
#include "modules/canbus/vehicle/niro/protocol/0d185_0xb9_steering_angle_report.h"
// Throttle protocols
#include "modules/canbus/vehicle/niro/protocol/0d144_0x90_throttle_enable.h"
#include "modules/canbus/vehicle/niro/protocol/0d145_0x91_throttle_disable.h"
#include "modules/canbus/vehicle/niro/protocol/0d146_0x92_throttle_command.h"
#include "modules/canbus/vehicle/niro/protocol/0d147_0x93_throttle_report.h"

// Brake pressure
#include "modules/canbus/vehicle/niro/protocol/0d544_0x220_brake_pressure.h"
// Steering angle
#include "modules/canbus/vehicle/niro/protocol/0d688_0x2B0_steering_angle.h"
// Wheel speed
#include "modules/canbus/vehicle/niro/protocol/0d902_0x386_wheel_speed.h"

namespace apollo {
namespace canbus {
namespace niro {

NiroMessageManager::NiroMessageManager() 
{
  #define NEED_CHECK true

  /**
   * @brief Control messages
   */
  // Brake protocols
  AddSendProtocolData<BrakeEnable_0x70, NEED_CHECK>();
  AddSendProtocolData<BrakeDisable_0x71, NEED_CHECK>();
  AddSendProtocolData<BrakeCommand_0x72, NEED_CHECK>();
  // Steering protocols
  AddSendProtocolData<SteeringEnable_0x80, NEED_CHECK>();
  AddSendProtocolData<SteeringDisable_0x81, NEED_CHECK>();
  AddSendProtocolData<SteeringTorqueCommand_0x82, NEED_CHECK>();
  AddSendProtocolData<SteeringAngleCommand_0xB8, NEED_CHECK>();
  // Throttle protocols
  AddSendProtocolData<ThrottleEnable_0x90, NEED_CHECK>();
  AddSendProtocolData<ThrottleDisable_0x91, NEED_CHECK>();
  AddSendProtocolData<ThrottleCommand_0x92, NEED_CHECK>();

  /**
   * @brief Report messages
   */
  // Brake protocols
  AddRecvProtocolData<BrakeReport_0x73, NEED_CHECK>();
  // Steering protocols
  AddRecvProtocolData<SteeringReport_0x83, NEED_CHECK>();
  // AddRecvProtocolData<SteeringAngleReport_0xB9, NEED_CHECK>();
  // Throttle protocols
  AddRecvProtocolData<ThrottleReport_0x93, NEED_CHECK>();

  AddRecvProtocolData<BrakePressure_0x220, NEED_CHECK>();
  AddRecvProtocolData<SteeringAngle_0x2B0, NEED_CHECK>();
  AddRecvProtocolData<WheelSpeed_0x386, NEED_CHECK>();
}

NiroMessageManager::~NiroMessageManager() {}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo

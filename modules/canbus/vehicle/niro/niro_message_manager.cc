#include "modules/canbus/vehicle/niro/niro_message_manager.h"

#include "modules/canbus/vehicle/niro/protocol/dec112_hex70_brake_enable.h"
#include "modules/canbus/vehicle/niro/protocol/dec113_hex71_brake_disable.h"
#include "modules/canbus/vehicle/niro/protocol/dec114_hex72_brake_command.h"
#include "modules/canbus/vehicle/niro/protocol/dec115_hex73_brake_report.h"

// #include "modules/canbus/vehicle/niro/protocol/dec544_hex220_brake_pressure.h"

namespace apollo {
namespace canbus {
namespace niro {

NiroMessageManager::NiroMessageManager() 
{
  // Control Messages
  AddSendProtocolData<BrakeEnable_0x70, true>();
  AddSendProtocolData<BrakeDisable_0x71, true>();
  AddSendProtocolData<BrakeCommand_0x72, true>();

  // Report Messages
  AddRecvProtocolData<BrakeReport_0x73, true>();
  // AddRecvProtocolData<BrakeReport_0x220, need_check>();
}

NiroMessageManager::~NiroMessageManager() {}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo

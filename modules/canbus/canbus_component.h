#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "cyber/common/macros.h"
#include "cyber/component/timer_component.h"
#include "cyber/cyber.h"
#include "cyber/timer/timer.h"

#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/proto/can_card_parameter.pb.h"
#include "modules/guardian/proto/guardian.pb.h"

#include "modules/canbus/vehicle/vehicle_controller.h"
#include "modules/common/monitor_log/monitor_log_buffer.h"
#include "modules/common/status/status.h"
#include "modules/control/proto/control_cmd.pb.h"
#include "modules/drivers/canbus/can_client/can_client.h"
#include "modules/drivers/canbus/can_comm/can_receiver.h"
#include "modules/drivers/canbus/can_comm/can_sender.h"
#include "modules/drivers/canbus/can_comm/message_manager.h"

namespace apollo {
namespace canbus {

class CanbusComponent final : public apollo::cyber::TimerComponent {
 public:
  CanbusComponent();

  std::string Name() const;

 private:
  bool Init() override;

  bool Proc() override;

  void Clear() override;

  void PublishChassis();

  void PublishChassisDetail();

  void OnControlCommand(const apollo::control::ControlCommand &control_command);

  void OnGuardianCommand(const apollo::guardian::GuardianCommand &guardian_command);

  apollo::common::Status OnError(const std::string &error_msg);

  void RegisterCanClients();

  CanbusConf canbus_conf_;

  std::shared_ptr<cyber::Reader<apollo::guardian::GuardianCommand>> guardian_cmd_reader_;

  std::shared_ptr<cyber::Reader<apollo::control::ControlCommand>> control_command_reader_;

  std::unique_ptr<apollo::drivers::canbus::CanClient> can_client_;

  CanSender<ChassisDetail> can_sender_;

  apollo::drivers::canbus::CanReceiver<ChassisDetail> can_receiver_;

  std::unique_ptr<MessageManager<ChassisDetail>> message_manager_;

  std::unique_ptr<VehicleController> vehicle_controller_;

  int64_t last_timestamp_ = 0;

  ::apollo::common::monitor::MonitorLogBuffer monitor_logger_buffer_;

  std::shared_ptr<cyber::Writer<Chassis>> chassis_writer_;

  std::shared_ptr<cyber::Writer<ChassisDetail>> chassis_detail_writer_;
};

CYBER_REGISTER_COMPONENT(CanbusComponent)

}  // namespace canbus
}  // namespace apollo

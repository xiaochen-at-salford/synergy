#pragma once

#include <memory>
#include <thread>

#include "modules/canbus/proto/canbus_conf.pb.h"
#include "modules/canbus/proto/chassis.pb.h"
#include "modules/canbus/proto/vehicle_parameter.pb.h"
// Brake command protocols
#include "modules/canbus/vehicle/niro/protocol/dec112_hex70_brake_enable.h"
#include "modules/canbus/vehicle/niro/protocol/dec113_hex71_brake_disable.h"
#include "modules/canbus/vehicle/niro/protocol/dec114_hex72_brake_command.h"
// Steering command protocols
#include "modules/canbus/vehicle/niro/protocol/dec128_hex80_steering_enable.h"
#include "modules/canbus/vehicle/niro/protocol/dec129_hex81_steering_disable.h"
#include "modules/canbus/vehicle/niro/protocol/dec130_hex82_steering_torque_command.h"
#include "modules/canbus/vehicle/niro/protocol/dec184_hexB8_steering_angle_command.h"
// Throttle command protocols 
#include "modules/canbus/vehicle/niro/protocol/dec144_hex90_throttle_enable.h"
#include "modules/canbus/vehicle/niro/protocol/dec145_hex91_throttle_disable.h"
#include "modules/canbus/vehicle/niro/protocol/dec146_hex92_throttle_command.h"

// #include "modules/canbus/vehicle/niro/protocol/dec544_hex220_brake_pressure.h"

#include "modules/canbus/vehicle/vehicle_controller.h"
#include "modules/common/proto/error_code.pb.h"
#include "modules/control/proto/control_cmd.pb.h"

namespace apollo {
namespace canbus {
namespace niro {

class NiroController final : public VehicleController {
 public:
  NiroController() {}

  virtual ~NiroController();

  ::apollo::common::ErrorCode Init(
      const VehicleParameter& params,
      CanSender<::apollo::canbus::ChassisDetail>* const can_sender,
      MessageManager<::apollo::canbus::ChassisDetail>* const message_manager )
      override ;

  bool Start() override;

  void Stop() override;

  Chassis chassis() override;
  
  //TODO(xiaochen):
  FRIEND_TEST(NiroControllerTest, SetDrivingMode);
  FRIEND_TEST(NiroControllerTest, Status);
  FRIEND_TEST(NiroControllerTest, UpdateDrivingMode);

 private:
  // main logical function for operation the car enter or exit the auto driving
  void Emergency() override;
  ::apollo::common::ErrorCode EnableAutoMode() override;
  ::apollo::common::ErrorCode DisableAutoMode() override;
  ::apollo::common::ErrorCode EnableSteeringOnlyMode() override;
  ::apollo::common::ErrorCode EnableSpeedOnlyMode() override;

  // NEUTRAL, REVERSE, DRIVE
  void Gear(Chassis::GearPosition state) override;

  // acceleration:0.00~99.99, unit:
  // acceleration_spd: 60 ~ 100, suggest: 90
  void Brake(double acceleration) override;

  // drive with old acceleration
  // gas:0.00~99.99 unit:
  void Throttle(double throttle) override;

  // drive with acceleration/deceleration
  // acc:-7.0~5.0 unit:m/s^2
  void Acceleration(double acc) override;

  // steering with old angle speed
  // angle:-99.99~0.00~99.99, unit:, left:+, right:-
  void Steer(double angle) override;

  // steering with new angle speed
  // angle:-99.99~0.00~99.99, unit:, left:+, right:-
  // angle_spd:0.00~99.99, unit:deg/s
  void Steer(double angle, double angle_spd) override;

  // set Electrical Park Brake
  void SetEpbBreak(const ::apollo::control::ControlCommand& command) override;
  void SetBeam(const ::apollo::control::ControlCommand& command) override;
  void SetHorn(const ::apollo::control::ControlCommand& command) override;
  void SetTurningSignal(const ::apollo::control::ControlCommand& command) override;

  void ResetProtocol();
  bool CheckChassisError();

 private:
  void SecurityDogThreadFunc();
  virtual bool CheckResponse(const int32_t flags, bool need_wait);
  void set_chassis_error_mask(const int32_t mask);
  int32_t chassis_error_mask();
  Chassis::ErrorCode chassis_error_code();
  void set_chassis_error_code(const Chassis::ErrorCode& error_code);

 private:
  // Brake command protocols
  BrakeEnable_0x70 *brake_enable_0x70_ = nullptr;
  BrakeDisable_0x71 *brake_disable_0x71_ = nullptr;
  BrakeCommand_0x72 *brake_command_0x72_ = nullptr;

  // Steeringc command protocols
  SteeringEnable_0x80 *steering_enable_0x80_ = nullptr;
  SteeringDisable_0x81 *steering_disable_0x81_ = nullptr;
  SteeringTorqueCommand_0x82 *steering_torque_command_0x82_ = nullptr;
  SteeringAngleCommand_0xB8  *steering_angle_command_0xB8_ = nullptr;

  // Throttle command protocols
  ThrottleEnable_0x90 *throttle_enable_0x90_ = nullptr;
  ThrottleDisable_0x91 *throttle_disable_0x91_ = nullptr;
  ThrottleCommand_0x92 *throttle_command_0x92_ = nullptr; // Throttle command protocols

  Chassis chassis_;
  std::unique_ptr<std::thread> thread_;
  bool is_chassis_error_ = false;

  std::mutex chassis_error_code_mutex_;
  Chassis::ErrorCode chassis_error_code_ = Chassis::NO_ERROR;

  std::mutex chassis_mask_mutex_;
  int32_t chassis_error_mask_ = 0;
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo

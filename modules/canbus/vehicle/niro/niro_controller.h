#pragma once

#include <memory>
#include <thread>

#include "modules/canbus/proto/canbus_conf.pb.h"
#include "modules/canbus/proto/chassis.pb.h"
#include "modules/canbus/proto/vehicle_parameter.pb.h"
#include "modules/canbus/vehicle/vehicle_controller.h"
#include "modules/common/proto/error_code.pb.h"
#include "modules/control/proto/control_cmd.pb.h"
#include "modules/canbus/vehicle/niro/protocol/0d112_0x70_brake_enable.h"
#include "modules/canbus/vehicle/niro/protocol/0d113_0x71_brake_disable.h"
#include "modules/canbus/vehicle/niro/protocol/0d114_0x72_brake_command.h"
#include "modules/canbus/vehicle/niro/protocol/0d128_0x80_steering_enable.h"
#include "modules/canbus/vehicle/niro/protocol/0d129_0x81_steering_disable.h"
#include "modules/canbus/vehicle/niro/protocol/0d130_0x82_steering_torque_command.h"
#include "modules/canbus/vehicle/niro/protocol/0d184_0xb8_steering_angle_command.h"
#include "modules/canbus/vehicle/niro/protocol/0d144_0x90_throttle_enable.h"
#include "modules/canbus/vehicle/niro/protocol/0d145_0x91_throttle_disable.h"
#include "modules/canbus/vehicle/niro/protocol/0d146_0x92_throttle_command.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::common::ErrorCode;
using ::apollo::canbus::ChassisDetail;

class NiroController final : public VehicleController {
 public:
  NiroController() {}
  virtual ~NiroController();

  ErrorCode Init(
      const VehicleParameter &params,
      CanSender<ChassisDetail>* const can_sender,
      MessageManager<ChassisDetail>* const message_manager ) override;

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
  ErrorCode EnableAutoMode() override;
  ErrorCode DisableAutoMode() override;
  ErrorCode EnableSteeringOnlyMode() override;
  ErrorCode EnableSpeedOnlyMode() override;

  void EnableOscc();
  void DisableOscc();
  void EnableOsccBrake();
  void DisableOsccBrake();
  void EnableOsccSteering();
  void DisableOsccSteering();
  void EnableOsccThrottle();
  void DisableOsccThrottle();

  void Gear(Chassis::GearPosition state) override 
  { AERROR << "API disabled for KIA Niro"; };

  /**
   * @brief Apply the percentage of brake pedal
   * @param percent [%]:[0.0, 100.0]
   */
  void Brake(double percent) override;

  /**
   * @brief Apply the percentage of throttle pedal
   * @param throttle_percent [%]:[0.0, 100.0]
   */
  void Throttle(double throttle_percent) override;

  void Acceleration(double acc) {}; // API disabled

  /**
   * @brief Apply the percentage of steering angle with the upper max angular velocity
   * @param angle_percent [%]:[-100.0, 100.0] := [degrees]:[-470.0, +470.0] 
   *                      "-":= right, "+":= left
   */
  void Steer(double angle_percent) override;

  /**
   * @brief Steer with specified max angular velocity
   */
  void Steer(double angle_percent, double max_angular_velocity_percent=100.0) override;

  /**
   * @brief Apply the percentage of steering angle with a customised max angular velocity
   * @param angle_percent [%]:[-100.0, 100.0] := [degrees]:[-600.0, +600.0]
   * @param max_angular_velocity_percent [%]:[0.0, 100.0] := [degrees/second]:[0.0, 800.0]
   */
  void SteerByAngle(double angle_percent, double angle_speed_percent);

  /**
   * @brief Apply the percentage of steering torque 
   * @param torque_percent [%]:[-100.0, +100.0]
   */
  void SteerByTorque(double torque_percent);

  // APIs disabled
  void SetEpbBreak(const ::apollo::control::ControlCommand& command) override {};
  void SetBeam(const ::apollo::control::ControlCommand& command) override {};
  void SetHorn(const ::apollo::control::ControlCommand& command) override {};
  void SetTurningSignal(const ::apollo::control::ControlCommand& command) override {}; 

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
  BrakeEnable_0x70 *brake_enable_ = nullptr;
  BrakeDisable_0x71 *brake_disable_ = nullptr;
  BrakeCommand_0x72 *brake_command_ = nullptr;
  SteeringEnable_0x80 *steering_enable_ = nullptr;
  SteeringDisable_0x81 *steering_disable_ = nullptr;
  SteeringTorqueCommand_0x82 *steering_torque_command_ = nullptr;
  SteeringAngleCommand_0xB8  *steering_angle_command_ = nullptr;
  ThrottleEnable_0x90 *throttle_enable_ = nullptr;
  ThrottleDisable_0x91 *throttle_disable_ = nullptr;
  ThrottleCommand_0x92 *throttle_command_ = nullptr; 

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

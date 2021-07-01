#include "modules/canbus/vehicle/niro/niro_controller.h"

#include "modules/common/proto/vehicle_signal.pb.h"

#include "cyber/common/log.h"
#include "cyber/time/time.h"
#include "modules/canbus/common/canbus_gflags.h"
#include "modules/canbus/vehicle/niro/niro_message_manager.h"
#include "modules/canbus/vehicle/vehicle_controller.h"
#include "modules/drivers/canbus/can_comm/can_sender.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::common::ErrorCode;
using ::apollo::control::ControlCommand;
using ::apollo::drivers::canbus::ProtocolData;

namespace {
const int32_t kMaxFailAttempt = 10;
const int32_t CHECK_RESPONSE_STEER_UNIT_FLAG = 1;
const int32_t CHECK_RESPONSE_SPEED_UNIT_FLAG = 2;
}  // namespace

ErrorCode NiroController::Init(
    const VehicleParameter& params,
    CanSender<::apollo::canbus::ChassisDetail>* const can_sender,
    MessageManager<::apollo::canbus::ChassisDetail>* const message_manager ) 
{
  if (is_initialized_) 
  {
    AINFO << "NiroController has already been initiated.";
    return ErrorCode::CANBUS_ERROR;
  }
  vehicle_params_.CopyFrom(common::VehicleConfigHelper::Instance()->GetConfig().vehicle_param());

  params_.CopyFrom(params);
  if (!params_.has_driving_mode()) 
  {
    AERROR << "Vehicle conf pb not set driving_mode.";
    return ErrorCode::CANBUS_ERROR;
  }

  if (can_sender == nullptr) 
  { return ErrorCode::CANBUS_ERROR; }
  can_sender_ = can_sender;

  if (message_manager == nullptr) 
  {
    AERROR << "protocol manager is null.";
    return ErrorCode::CANBUS_ERROR;
  }
  message_manager_ = message_manager;

  // Sender part
  brake_enable_ = dynamic_cast<BrakeEnable_0x70*>(
      message_manager_->GetMutableProtocolDataById(BrakeEnable_0x70::ID) );
  if (brake_enable_ == nullptr) 
  {
    AERROR << "BrakeEnable_0x70 does not exist in the OsccMessageManager!";
    return ErrorCode::CANBUS_ERROR;
  }

  brake_disable_ = dynamic_cast<BrakeDisable_0x71*>(
      message_manager_->GetMutableProtocolDataById(BrakeDisable_0x71::ID) );
  if (brake_disable_ == nullptr) 
  {
    AERROR << "BrakeDisable_0x71 does not exist in the OsccMessageManager!";
    return ErrorCode::CANBUS_ERROR;
  }

  brake_command_ = dynamic_cast<BrakeCommand_0x72*>(
      message_manager_->GetMutableProtocolDataById(BrakeCommand_0x72::ID) );
  if (brake_command_ == nullptr) 
  {
    AERROR << "BrakeCommand_0x72 does not exist in the OsccMessageManager!";
    return ErrorCode::CANBUS_ERROR;
  }

  steering_enable_ = dynamic_cast<SteeringEnable_0x80*>(
      message_manager_->GetMutableProtocolDataById(SteeringEnable_0x80::ID) );
  if (steering_enable_ == nullptr) 
  {
    AERROR << "SteeringEnable_0x80 does not exist in the OsccMessageManager!";
    return ErrorCode::CANBUS_ERROR;
  }

  steering_disable_ = dynamic_cast<SteeringDisable_0x81*>(
      message_manager_->GetMutableProtocolDataById(SteeringDisable_0x81::ID) );
  if (steering_disable_ == nullptr) 
  {
    AERROR << "SteeringDisable_0x81 does not exist in the OsccMessageManager!";
    return ErrorCode::CANBUS_ERROR;
  }

  // steering_torque_command_ = dynamic_cast<SteeringTorqueCommand_0x82*>(
  //     message_manager_->GetMutableProtocolDataById(SteeringTorqueCommand_0x82::ID) );
  // if (steering_torque_command_ == nullptr) 
  // {
  //   AERROR << "SteeringTorqueCommand_0x82 does not exist in the OsccMessageManager!";
  //   return ErrorCode::CANBUS_ERROR;
  // }

  steering_angle_command_ = dynamic_cast<SteeringAngleCommand_0xB8*>(
      message_manager_->GetMutableProtocolDataById(SteeringAngleCommand_0xB8::ID) );
  if (steering_angle_command_ == nullptr) 
  {
    AERROR << "SteeringTorqueCommand_0xB8 does not exist in the OsccMessageManager!";
    return ErrorCode::CANBUS_ERROR;
  }

  throttle_enable_ = dynamic_cast<ThrottleEnable_0x90*>(
      message_manager_->GetMutableProtocolDataById(ThrottleEnable_0x90::ID) );
  if (throttle_enable_ == nullptr) 
  {
    AERROR << "ThrottleEnable_0x90 does not exist in the OsccMessageManager!";
    return ErrorCode::CANBUS_ERROR;
  }

  throttle_disable_ = dynamic_cast<ThrottleDisable_0x91*>(
      message_manager_->GetMutableProtocolDataById(ThrottleDisable_0x91::ID) );
  if (throttle_disable_ == nullptr) 
  {
    AERROR << "ThrottleDisable_0x92 does not exist in the OsccMessageManager!";
    return ErrorCode::CANBUS_ERROR;
  }

  throttle_command_ = dynamic_cast<ThrottleCommand_0x92*>(
      message_manager_->GetMutableProtocolDataById(ThrottleCommand_0x92::ID) );
  if (throttle_command_ == nullptr) 
  {
    AERROR << "ThrottleCommand_0x92 does not exist in the OsccMessageManager!";
    return ErrorCode::CANBUS_ERROR;
  }

  // Brake command messages
  can_sender_->AddMessage(BrakeEnable_0x70::ID, brake_enable_, false);
  can_sender_->AddMessage(BrakeDisable_0x71::ID, brake_disable_, false);
  can_sender_->AddMessage(BrakeCommand_0x72::ID, brake_command_, false);
  // Steering command messages
  can_sender_->AddMessage(SteeringEnable_0x80::ID, steering_enable_, false);
  can_sender_->AddMessage(SteeringDisable_0x81::ID, steering_disable_, false);
  can_sender_->AddMessage(SteeringTorqueCommand_0x82::ID, steering_torque_command_, false);
  can_sender_->AddMessage(SteeringAngleCommand_0xB8::ID, steering_angle_command_, false);
  // Throttle command messages
  can_sender_->AddMessage(ThrottleEnable_0x90::ID, throttle_enable_, false);
  can_sender_->AddMessage(ThrottleDisable_0x91::ID, throttle_disable_, false);
  can_sender_->AddMessage(ThrottleCommand_0x92::ID, throttle_command_, false);

  // need sleep to ensure all messages received
  AINFO << "NiroController is initialized.";

  is_initialized_ = true;
  return ErrorCode::OK;
}

NiroController::~NiroController() {}

bool NiroController::Start() 
{
  if (!is_initialized_) 
  {
    AERROR << "NiroController has NOT been initiated.";
    return false;
  }
  const auto& update_func = [this] { SecurityDogThreadFunc(); };
  thread_.reset(new std::thread(update_func));

  return true;
}

void NiroController::Stop() 
{
  if (!is_initialized_) 
  {
    AERROR << "NiroController stops or starts improperly!";
    return;
  }

  if (thread_ != nullptr && thread_->joinable()) 
  {
    thread_->join();
    thread_.reset();
    AINFO << "NiroController stopped.";
  }
}

//TODO(xiaochen)
Chassis NiroController::chassis() 
{
  chassis_.Clear();

  ChassisDetail chassis_detail;
  message_manager_->GetSensorData(&chassis_detail);

  // 21, 22, previously 1, 2
  if (driving_mode() == Chassis::EMERGENCY_MODE) 
  { set_chassis_error_code(Chassis::NO_ERROR); }

  // Driving mode
  chassis_.set_driving_mode(driving_mode());
  chassis_.set_error_code(chassis_error_code());

  // Brake
  // if (chassis_detail.niro().has_brake_report_() 
  //     && chassis_detail.niro().brake_report_0x73().has_brake_pedal_actual()) 
  // {
  //   chassis_.set_brake_percentage(static_cast<float>(
  //       chassis_detail.niro().brake_report_0x73().brake_pedal_actual()));
  // } 
  // else 
  // { chassis_.set_brake_percentage(0); }
  chassis_.set_brake_percentage(-1);

  // Steering
  // if (chassis_detail.niro().has_steering_report() 
  //     && chassis_detail.devkit().steering_report().has_steer_angle_actual() ) 
  // {
  //   chassis_.set_steering_percentage(static_cast<float>(
  //       chassis_detail.devkit().steering_report_502().steer_angle_actual() *
  //       100.0 / vehicle_params_.max_steer_angle() * M_PI / 180 ) );
  // } 
  // else 
  // { chassis_.set_steering_percentage(0); }

  // Throttle
  // if (chassis_detail.niro().has_throttle_report_() 
  //     && chassis_detail.devkit().throttle_report_().has_throttle_pedal_actual() ) 
  // {
  //   chassis_.set_throttle_percentage(static_cast<float>(
  //       chassis_detail.devkit().throttle_report_500().throttle_pedal_actual()));
  // } 
  // else 
  // {
  //   chassis_.set_throttle_percentage(0);
  // }

  // Wheel
  // if (chassis_detail.niro().has_wheelspeed_report_506()) 
  // {
  //   if (chassis_detail.niro().wheelspeed_report_506().has_rr()) 
  //   {
  //     chassis_.mutable_wheel_speed()->set_wheel_spd_rr(
  //         chassis_detail.niro().wheelspeed_report_506().rr());
  //   }
  //   if (chassis_detail.niro().wheelspeed_report_506().has_rl()) 
  //   {
  //     chassis_.mutable_wheel_speed()->set_wheel_spd_rl(
  //         chassis_detail.niro().wheelspeed_report_506().rl());
  //   }
  //   if (chassis_detail.niro().wheelspeed_report_506().has_fr()) 
  //   {
  //     chassis_.mutable_wheel_speed()->set_wheel_spd_fr(
  //         chassis_detail.devkit().wheelspeed_report_506().fr());
  //   }
  //   if (chassis_detail.devkit().wheelspeed_report_506().has_fl()) {
  //     chassis_.mutable_wheel_speed()->set_wheel_spd_fl(
  //         chassis_detail.devkit().wheelspeed_report_506().fl());
  //   }
  // }

  return chassis_;
}

void NiroController::Emergency() 
{
  set_driving_mode(Chassis::EMERGENCY_MODE);
  ResetProtocol();
}

ErrorCode NiroController::EnableAutoMode() 
{
  if (driving_mode() == Chassis::COMPLETE_AUTO_DRIVE) 
  {
    AINFO << "already in COMPLETE_AUTO_DRIVE mode";
    return ErrorCode::OK;
  }

  EnableOscc();

  can_sender_->Update();

  const int32_t flag = CHECK_RESPONSE_STEER_UNIT_FLAG | CHECK_RESPONSE_SPEED_UNIT_FLAG;
  if (!CheckResponse(flag, true)) 
  {
    AERROR << "Failed to switch to COMPLETE_AUTO_DRIVE mode.";
    Emergency();
    set_chassis_error_code(Chassis::CHASSIS_ERROR);
    return ErrorCode::CANBUS_ERROR;
  }
  set_driving_mode(Chassis::COMPLETE_AUTO_DRIVE);
  AINFO << "Switch to COMPLETE_AUTO_DRIVE mode ok.";
  return ErrorCode::OK;
}

ErrorCode NiroController::DisableAutoMode() 
{
  ResetProtocol();
  can_sender_->Update();
  set_driving_mode(Chassis::COMPLETE_MANUAL);
  set_chassis_error_code(Chassis::NO_ERROR);
  AINFO << "Switch to COMPLETE_MANUAL ok.";
  return ErrorCode::OK;
}

ErrorCode NiroController::EnableSpeedOnlyMode() 
{
  if (driving_mode() == Chassis::COMPLETE_AUTO_DRIVE 
      || driving_mode() == Chassis::AUTO_STEER_ONLY ) 
  {
    set_driving_mode(Chassis::AUTO_STEER_ONLY);
    AINFO << "Already in AUTO_STEER_ONLY mode.";
    return ErrorCode::OK;
  }
  AFATAL << "SpeedOnlyMode is not supported in devkit!";
  return ErrorCode::CANBUS_ERROR;
}

ErrorCode NiroController::EnableSteeringOnlyMode() 
{
  if (driving_mode() == Chassis::COMPLETE_AUTO_DRIVE ||
      driving_mode() == Chassis::AUTO_STEER_ONLY) {
    set_driving_mode(Chassis::AUTO_STEER_ONLY);
    AINFO << "Already in AUTO_STEER_ONLY mode.";
    return ErrorCode::OK;
  }
  AFATAL << "SpeedOnlyMode is not supported in devkit!";
  return ErrorCode::CANBUS_ERROR;
}

void NiroController::EnableOscc()
{
  EnableOsccBrake();
  EnableOsccSteering();
  EnableOsccThrottle();
}

void NiroController::DisableOscc()
{
  DisableOsccBrake();
  DisableOsccSteering();
  DisableOsccThrottle();
}

void NiroController::EnableOsccBrake()
{ brake_enable_->activate(); }

void NiroController::DisableOsccBrake()
{ brake_disable_->activate(); }

void NiroController::EnableOsccSteering()
{ steering_enable_->activate(); }

void NiroController::DisableOsccSteering()
{ steering_disable_->activate(); }

void NiroController::EnableOsccThrottle()
{ throttle_enable_->activate(); }

void NiroController::DisableOsccThrottle()
{ throttle_disable_->activate(); }

void NiroController::Brake(double brake_percent) 
{ 
  brake_command_->set_brake_pedal_command(brake_percent); 
}

void NiroController::Throttle(double throttle_percent) 
{
  // if (driving_mode() != Chassis::COMPLETE_AUTO_DRIVE 
  //     && driving_mode() != Chassis::AUTO_SPEED_ONLY ) 
  // {
  //   AINFO << "The current drive mode does not need to set throttle pedal.";
  //   return;
  // }
  throttle_command_->set_throttle_pedal_command(throttle_percent);
}

void NiroController::Steer(double angle_percent) 
{ SteerByAngle(angle_percent, 100.0); }

void NiroController::Steer(double angle, double angle_speed) 
{ SteerByAngle(angle,angle_speed); }

//TODO(xiaochen):
void NiroController::SteerByAngle(double angle_percent, double max_angular_velocity_percent) 
{
  // if (driving_mode() != Chassis::COMPLETE_AUTO_DRIVE 
  //     && driving_mode() != Chassis::AUTO_STEER_ONLY ) 
  // {
  //   AINFO << "The current driving mode does not need to set steer.";
  //   return;
  // }
  // printf("steering by angle called\n");
  steering_angle_command_->set_steering_angle_command_flags(true);
  steering_angle_command_->set_steering_angle_percent(angle_percent);
  steering_angle_command_->set_max_steering_velocity_percent(max_angular_velocity_percent);
}

void NiroController::SteerByTorque(double torque_percent) 
{
  if (driving_mode() != Chassis::COMPLETE_AUTO_DRIVE && driving_mode() != Chassis::AUTO_STEER_ONLY) 
  {
    AINFO << "The current driving mode does not need to set steer.";
    return;
  }
  steering_torque_command_->set_steering_torque_percent(torque_percent);
}

void NiroController::ResetProtocol() 
{ message_manager_->ResetSendMessages(); }

bool NiroController::CheckChassisError() 
{
  ChassisDetail chassis_detail;
  message_manager_->GetSensorData(&chassis_detail);
  if (!chassis_detail.has_niro()) 
  {
    AERROR_EVERY(100) << "ChassisDetail has no niro vehicle info."
                      << chassis_detail.DebugString() ;
    return false;
  }

  Niro niro = chassis_detail.niro();

  // steer fault
  //TODO(xiaochen): wip...
  // if (niro.has_steering_report_0x83()) 
  // {
  //   if (Steering_report_502::STEER_FLT1_STEER_SYSTEM_HARDWARE_FAULT ==
  //       niro.steering_report_0x().steer_flt1() ) 
  //   { return true; }
  // }
  // // drive fault
  // if (devkit.has_throttle_report_500()) {
  //   if (Throttle_report_500::THROTTLE_FLT1_DRIVE_SYSTEM_HARDWARE_FAULT ==
  //       devkit.throttle_report_500().throttle_flt1()) {
  //     return true;
  //   }
  // }
  // brake fault
  // if (niro.has_brake_report_()) {
  //   return true;
  //   // if (Brake_Report_0x73::BRAKE_FLT1_BRAKE_SYSTEM_HARDWARE_FAULT == niro.brake_report_0x73().brake_flt1()) 
  //   // { return true; }
  // }

  return false;
}

void NiroController::SecurityDogThreadFunc() 
{
  int32_t vertical_ctrl_fail = 0;
  int32_t horizontal_ctrl_fail = 0;

  if (can_sender_ == nullptr) 
  {
    AERROR << "Failed to run SecurityDogThreadFunc() because can_sender_ is "
              "nullptr.";
    return;
  }

  while (!can_sender_->IsRunning()) 
  { std::this_thread::yield(); }

  std::chrono::duration<double, std::micro> default_period{50000};
  int64_t start = 0;
  int64_t end = 0;
  while (can_sender_->IsRunning()) 
  {
    start = ::apollo::cyber::Time::Now().ToMicrosecond();
    const Chassis::DrivingMode mode = driving_mode();
    bool emergency_mode = false;

    // 1. horizontal control check
    if ((mode == Chassis::COMPLETE_AUTO_DRIVE || mode == Chassis::AUTO_STEER_ONLY) 
        && CheckResponse(CHECK_RESPONSE_STEER_UNIT_FLAG, false) == false ) 
    {
      ++horizontal_ctrl_fail;
      if (horizontal_ctrl_fail >= kMaxFailAttempt) 
      {
        emergency_mode = true;
        set_chassis_error_code(Chassis::MANUAL_INTERVENTION);
      }
    } 
    else 
    { horizontal_ctrl_fail = 0; }

    // 2. vertical control check
    if ((mode == Chassis::COMPLETE_AUTO_DRIVE 
        || mode == Chassis::AUTO_SPEED_ONLY) 
        && !CheckResponse(CHECK_RESPONSE_SPEED_UNIT_FLAG, false) ) 
    {
      ++vertical_ctrl_fail;
      if (vertical_ctrl_fail >= kMaxFailAttempt) 
      {
        emergency_mode = true;
        set_chassis_error_code(Chassis::MANUAL_INTERVENTION);
      }
    } 
    else 
    { vertical_ctrl_fail = 0; }
    if (CheckChassisError()) 
    {
      set_chassis_error_code(Chassis::CHASSIS_ERROR);
      emergency_mode = true;
    }

    if (emergency_mode && mode != Chassis::EMERGENCY_MODE) 
    {
      set_driving_mode(Chassis::EMERGENCY_MODE);
      message_manager_->ResetSendMessages();
    }
    end = ::apollo::cyber::Time::Now().ToMicrosecond();
    std::chrono::duration<double, std::micro> elapsed{end - start};
    if (elapsed < default_period) 
    { std::this_thread::sleep_for(default_period - elapsed); } 
    else 
    {
      AERROR << "Too much time consumption in DevkitController looping process:"
             << elapsed.count();
    }
  }
}

bool NiroController::CheckResponse(const int32_t flags, bool need_wait) 
{
  int32_t retry_num = 20;
  ChassisDetail chassis_detail;
  bool is_eps_online = false;
  bool is_vcu_online = false;
  bool is_esp_online = false;

  do 
  {
    if (message_manager_->GetSensorData(&chassis_detail) != ErrorCode::OK) 
    {
      AERROR_EVERY(100) << "get chassis detail failed.";
      return false;
    }
    bool check_ok = true;
    if (flags & CHECK_RESPONSE_STEER_UNIT_FLAG) 
    {
      is_eps_online = chassis_detail.has_check_response() 
                      && chassis_detail.check_response().has_is_eps_online() 
                      && chassis_detail.check_response().is_eps_online();
      check_ok = check_ok && is_eps_online;
    }

    if (flags & CHECK_RESPONSE_SPEED_UNIT_FLAG) 
    {
      is_vcu_online = chassis_detail.has_check_response() 
                      && chassis_detail.check_response().has_is_vcu_online() 
                      && chassis_detail.check_response().is_vcu_online();
      is_esp_online = chassis_detail.has_check_response() 
                      && chassis_detail.check_response().has_is_esp_online() 
                      && chassis_detail.check_response().is_esp_online();
      check_ok = check_ok && is_vcu_online && is_esp_online;
    }
    if (check_ok) 
    { return true; } 
    else 
    { AINFO << "Need to check response again."; }
    if (need_wait) 
    {
      --retry_num;
      std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(20));
    }
  } 
  while (need_wait && retry_num);

  AINFO << "check_response fail: is_eps_online:" << is_eps_online
        << ", is_vcu_online:" << is_vcu_online
        << ", is_esp_online:" << is_esp_online ;

  return false;
}

void NiroController::set_chassis_error_mask(const int32_t mask) 
{
  std::lock_guard<std::mutex> lock(chassis_mask_mutex_);
  chassis_error_mask_ = mask;
}

int32_t NiroController::chassis_error_mask() 
{
  std::lock_guard<std::mutex> lock(chassis_mask_mutex_);
  return chassis_error_mask_;
}

Chassis::ErrorCode NiroController::chassis_error_code() 
{
  std::lock_guard<std::mutex> lock(chassis_error_code_mutex_);
  return chassis_error_code_;
}

void NiroController::set_chassis_error_code(const Chassis::ErrorCode& error_code) 
{
  std::lock_guard<std::mutex> lock(chassis_error_code_mutex_);
  chassis_error_code_ = error_code;
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo

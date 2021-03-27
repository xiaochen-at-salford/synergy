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
  brake_enable_0x70_ = dynamic_cast<BrakeEnable_0x70*>(
      message_manager_->GetMutableProtocolDataById(BrakeEnable_0x70::ID) );
  if (brake_enable_0x70_ == nullptr) 
  {
    AERROR << "BrakeCommand_0x70 does not exist in the OsccMessageManager!";
    return ErrorCode::CANBUS_ERROR;
  }

  brake_disable_0x71_ = dynamic_cast<BrakeDisable_0x71*>(
      message_manager_->GetMutableProtocolDataById(BrakeDisable_0x71::ID) );
  if (brake_disable_0x71_ == nullptr) 
  {
    AERROR << "BrakeCommand70 does not exist in the OsccMessageManager!";
    return ErrorCode::CANBUS_ERROR;
  }

  brake_command_0x72_ = dynamic_cast<BrakeCommand_0x72*>(
      message_manager_->GetMutableProtocolDataById(BrakeCommand_0x72::ID) );
  if (brake_command_0x72_ == nullptr) 
  {
    AERROR << "BrakeCommand70 does not exist in the OsccMessageManager!";
    return ErrorCode::CANBUS_ERROR;
  }

  bool &&init_with_one = false;
  can_sender_->AddMessage(BrakeEnable_0x70::ID, brake_enable_0x70_, init_with_one);
  can_sender_->AddMessage(BrakeDisable_0x71::ID, brake_disable_0x71_, init_with_one);
  can_sender_->AddMessage(BrakeCommand_0x72::ID, brake_command_0x72_, init_with_one);

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

//wip... 
Chassis NiroController::chassis() 
{
  chassis_.Clear();

  ChassisDetail chassis_detail;
  message_manager_->GetSensorData(&chassis_detail);

  // 21, 22, previously 1, 2
  if (driving_mode() == Chassis::EMERGENCY_MODE) 
  { set_chassis_error_code(Chassis::NO_ERROR); }

  chassis_.set_driving_mode(driving_mode());
  chassis_.set_error_code(chassis_error_code());

  // 3
  chassis_.set_engine_started(true);

  // 4 engine rpm ch has no engine rpm
  // chassis_.set_engine_rpm(0);

  // 5 wheel spd
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

  // 6 speed_mps
  // if (chassis_detail.devkit().has_vcu_report_505() 
  //     && chassis_detail.devkit().vcu_report_505().has_speed() ) 
  // {
  //   chassis_.set_speed_mps(static_cast<float>(chassis_detail.devkit().vcu_report_505().speed()));
  // } 
  // else 
  // { chassis_.set_speed_mps(0); }

  // 7 no odometer
  // chassis_.set_odometer_m(0);

  // 8 no fuel. do not set;
  // chassis_.set_fuel_range_m(0);

  // 9 throttle
//   if (chassis_detail.devkit().has_throttle_report_500() 
//       && chassis_detail.devkit().throttle_report_500().has_throttle_pedal_actual() ) 
//  {
//     chassis_.set_throttle_percentage(static_cast<float>(
//         chassis_detail.devkit().throttle_report_500().throttle_pedal_actual()));
//   } 
//   else 
//   {
//     chassis_.set_throttle_percentage(0);
//   }

  // 10 brake
  // if (chassis_detail.niro().has_brake_report_() 
  //     && chassis_detail.niro().brake_report_0x73().has_brake_pedal_actual()) 
  // {
  //   chassis_.set_brake_percentage(static_cast<float>(
  //       chassis_detail.niro().brake_report_0x73().brake_pedal_actual()));
  // } 
  // else 
  // { chassis_.set_brake_percentage(0); }
  chassis_.set_brake_percentage(0);

  // 23, previously 11 gear
  // if (chassis_detail.devkit().has_gear_report_503() 
  //     && chassis_detail.devkit().gear_report_503().has_gear_actual() ) 
  // {
  //   Chassis::GearPosition gear_pos = Chassis::GEAR_INVALID;

  //   if (chassis_detail.devkit().gear_report_503().gear_actual() ==
  //       Gear_report_503::GEAR_ACTUAL_INVALID) 
  //   {
  //     gear_pos = Chassis::GEAR_INVALID;
  //   }
  //   if (chassis_detail.devkit().gear_report_503().gear_actual() ==
  //       Gear_report_503::GEAR_ACTUAL_NEUTRAL) {
  //     gear_pos = Chassis::GEAR_NEUTRAL;
  //   }
  //   if (chassis_detail.devkit().gear_report_503().gear_actual() ==
  //       Gear_report_503::GEAR_ACTUAL_REVERSE) 
  //   {
  //     gear_pos = Chassis::GEAR_REVERSE;
  //   }
  //   if (chassis_detail.devkit().gear_report_503().gear_actual() ==
  //       Gear_report_503::GEAR_ACTUAL_DRIVE) {
  //     gear_pos = Chassis::GEAR_DRIVE;
  //   }
  //   if (chassis_detail.devkit().gear_report_503().gear_actual() ==
  //       Gear_report_503::GEAR_ACTUAL_PARK) {
  //     gear_pos = Chassis::GEAR_PARKING;
  //   }
  //   chassis_.set_gear_location(gear_pos);
  // } 
  // else 
  // { chassis_.set_gear_location(Chassis::GEAR_NONE); }

  // 12 steering
  // if (chassis_detail.devkit().has_steering_report_502() 
  //     && chassis_detail.devkit().steering_report_502().has_steer_angle_actual() ) 
  // {
  //   chassis_.set_steering_percentage(static_cast<float>(
  //       chassis_detail.devkit().steering_report_502().steer_angle_actual() *
  //       100.0 / vehicle_params_.max_steer_angle() * M_PI / 180 ) );
  // } 
  // else 
  // { chassis_.set_steering_percentage(0); }
  chassis_.set_steering_percentage(0); 

  // 13 parking brake
  // if (chassis_detail.devkit().has_park_report_504() 
  //     && chassis_detail.devkit().park_report_504().has_parking_actual()) 
  // {
  //   if (chassis_detail.devkit().park_report_504().parking_actual() == 
  //           Park_report_504::PARKING_ACTUAL_PARKING_TRIGGER ) 
  //   {
  //     chassis_.set_parking_brake(true);
  //   } 
  //   else 
  //   { chassis_.set_parking_brake(false); }
  // } 
  // else 
  // { chassis_.set_parking_brake(false); }

  // 14 battery soc
  // if (chassis_detail.devkit().has_bms_report_512() 
  //     && chassis_detail.devkit().bms_report_512().has_battery_soc() ) 
  // {
  //   chassis_.set_battery_soc_percentage(
  //       chassis_detail.devkit().bms_report_512().battery_soc() );
  // } 
  // else 
  // { chassis_.set_battery_soc_percentage(0); }

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
  // set enable
  brake_enable_0x70_->set_brake_enable();

  //TODO(xiaochen): 
  // throttle_command_0x_->set_throttle_enable();
  // steering_command_0x_->set_steer_enable();

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

ErrorCode NiroController::EnableSteeringOnlyMode() 
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

// brake with pedal
// pedal:0.00~99.99, unit:%
void NiroController::Brake(double pedal) 
{
  // double real_value = params_.max_acc() * acceleration / 100;
  // TODO(All) :  Update brake value based on mode
  if (driving_mode() != Chassis::COMPLETE_AUTO_DRIVE 
      && driving_mode() != Chassis::AUTO_SPEED_ONLY ) 
  {
    AINFO << "The current drive mode does not need to set brake pedal.";
    return;
  }
  brake_command_0x72_->set_brake_pedal_command(pedal);
}

// drive with pedal
// pedal:0.0~99.9 unit:%
void NiroController::Throttle(double pedal) 
{
  if (driving_mode() != Chassis::COMPLETE_AUTO_DRIVE 
      && driving_mode() != Chassis::AUTO_SPEED_ONLY ) 
  {
    AINFO << "The current drive mode does not need to set throttle pedal.";
    return;
  }
  //TODO(xiaochen): wip...
  // throttle_command_0x92_->set_throttle_pedal_target(pedal);
}

// confirm the car is driven by acceleration command instead of throttle/brake
// pedal drive with acceleration/deceleration acc:-7.0 ~ 5.0, unit:m/s^2
void NiroController::Acceleration(double acc) 
{
  if (driving_mode() != Chassis::COMPLETE_AUTO_DRIVE 
      || driving_mode() != Chassis::AUTO_SPEED_ONLY ) 
  {
    AINFO << "The current drive mode does not need to set acceleration.";
    return;
  }
  // None
}

// devkit default, -30 ~ 00, left:+, right:-
// need to be compatible with control module, so reverse
// steering with default angle speed, 25-250 (default:250)
// angle:-99.99~0.00~99.99, unit:, left:-, right:+
void NiroController::Steer(double angle) 
{
  // if (driving_mode() != Chassis::COMPLETE_AUTO_DRIVE 
  //     && driving_mode() != Chassis::AUTO_STEER_ONLY ) 
  // {
  //   AINFO << "The current driving mode does not need to set steer.";
  //   return;
  // }
  // const double real_angle = vehicle_params_.max_steer_angle()/M_PI*180*angle/100.0;
  // //TODO(xiaochen): wip...
  // steering_command_102_->set_steer_angle_target(real_angle)->set_steer_angle_spd(250);
}

// steering with new angle speed
// angle:-99.99~0.00~99.99, unit:, left:-, right:+
// angle_spd:25~250, unit:deg/s
void NiroController::Steer(double angle, double angle_spd) 
{
  // if (driving_mode() != Chassis::COMPLETE_AUTO_DRIVE &&
  //     driving_mode() != Chassis::AUTO_STEER_ONLY) 
  // {
  //   AINFO << "The current driving mode does not need to set steer.";
  //   return;
  // }
  // const double real_angle = vehicle_params_.max_steer_angle()/M_PI*180*angle/100.0;
  //TODO(xiaochen): wip...
  // steering_command_102_->set_steer_angle_target(real_angle)->set_steer_angle_spd(250) ;
}

void NiroController::SetEpbBreak(const ControlCommand& command) 
{
  return;
  // if (command.parking_brake()) 
  // {
  //   // None
  // } else {
  //   // None
  // }
}

void NiroController::SetBeam(const ControlCommand& command) 
{
  return;
  // if (command.signal().high_beam()) {
  //   // None
  // } else if (command.signal().low_beam()) {
  //   // None
  // } else {
  //   // None
  // }
}

void NiroController::SetHorn(const ControlCommand& command) 
{
  return;
  // if (command.signal().horn()) {
  //   // None
  // } else {
  //   // None
  // }
}

void NiroController::SetTurningSignal(const ControlCommand& command) 
{
  // Set Turn Signal do not support on devkit
}

void NiroController::ResetProtocol() 
{
  message_manager_->ResetSendMessages();
}

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

  // Devkit devkit = chassis_detail.devkit();
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
  if (niro.has_brake_report_0x73()) {
    return true;
    // if (Brake_Report_0x73::BRAKE_FLT1_BRAKE_SYSTEM_HARDWARE_FAULT == niro.brake_report_0x73().brake_flt1()) 
    // { return true; }
  }

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
    {
      horizontal_ctrl_fail = 0;
    }

    // 2. vertical control check
    if ((mode == Chassis::COMPLETE_AUTO_DRIVE || mode == Chassis::AUTO_SPEED_ONLY) 
        && !CheckResponse(CHECK_RESPONSE_SPEED_UNIT_FLAG, false ) ) 
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

#include "modules/canbus/vehicle/vehicle_controller.h"

#include "cyber/common/log.h"

namespace apollo {
namespace canbus {

using common::ErrorCode;
using control::ControlCommand;

Chassis::DrivingMode VehicleController::driving_mode() 
{
  std::lock_guard<std::mutex> lock(mode_mutex_);
  return driving_mode_;
}

void VehicleController::set_driving_mode(const Chassis::DrivingMode &driving_mode) 
{
  std::lock_guard<std::mutex> lock(mode_mutex_);
  driving_mode_ = driving_mode;
}

ErrorCode VehicleController::SetDrivingMode(const Chassis::DrivingMode &driving_mode) 
{
  if (driving_mode == Chassis::EMERGENCY_MODE) 
  {
    AINFO << "Can't set vehicle to EMERGENCY_MODE driving mode.";
    return ErrorCode::CANBUS_ERROR;
  }

  // vehicle in emergency mode only response to manual mode to reset.
  if (this->driving_mode() == Chassis::EMERGENCY_MODE && driving_mode != Chassis::COMPLETE_MANUAL) 
  {
    AINFO << "Vehicle in EMERGENCY_MODE, only response to COMPLETE_MANUAL mode.";
    AINFO << "Only response to RESET ACTION.";
    return ErrorCode::CANBUS_ERROR;
  }

  // if current mode is same as previous, no need to set.
  if (this->driving_mode() == driving_mode) 
  { return ErrorCode::OK; }

  switch (driving_mode) 
  {
    case Chassis::COMPLETE_AUTO_DRIVE: 
      if (EnableAutoMode() != ErrorCode::OK) 
      {
        AERROR << "Failed to enable auto mode.";
        return ErrorCode::CANBUS_ERROR;
      }
      break;

    case Chassis::COMPLETE_MANUAL: 
      if (DisableAutoMode() != ErrorCode::OK) 
      {
        AERROR << "Failed to disable auto mode.";
        return ErrorCode::CANBUS_ERROR;
      }
      break;

    case Chassis::AUTO_STEER_ONLY: 
      if (EnableSteeringOnlyMode() != ErrorCode::OK) 
      {
        AERROR << "Failed to enable steer only mode.";
        return ErrorCode::CANBUS_ERROR;
      }
      break;

    case Chassis::AUTO_SPEED_ONLY: 
      if (EnableSpeedOnlyMode() != ErrorCode::OK) 
      {
        AERROR << "Failed to enable speed only mode";
        return ErrorCode::CANBUS_ERROR;
      }
      break;

    default:
      break;
  }
  return ErrorCode::OK;
}

ErrorCode VehicleController::Update(const ControlCommand &control_command) 
{
  if (!is_initialized_) 
  {
    AERROR << "Controller is not initialized.";
    return ErrorCode::CANBUS_ERROR;
  }
  //TODO(xiaochen): Add OSCC control
  if (control_command.has_oscc_msg())
  {
    // printf("Received OSCC control command from keyboard\n");
    AINFO << "CAN bus received OSCC messages: "
          << control_command.oscc_msg().ShortDebugString() ;
    switch (control_command.oscc_msg().action())
    {
      case control::OsccAction::ENABLE_ALL:
        printf("Received OSCC_ENABLE_ALL command from the keyboard\n");
        EnableOscc(); 
        break;

      case control::OsccAction::DISABLE_ALL:
        printf("Received OSCC_DISABLE_ALL command from the keyboard\n");
        DisableOscc();
        break;
      
      case control::OsccAction::ENABLE_BRAKE:
        printf("Received OSCC_ENABLE_BRAKE command from the keyboard\n");
        EnableOsccBrake();
        break;

      case control::OsccAction::DISABLE_BRAKE:
        printf("Received OSCC_DISABLE_BRAKE command from the keyboard\n");
        DisableOsccBrake();
        break;

      case control::OsccAction::ENABLE_STEERING:
        printf("Received OSCC_ENABLE_STEERING command from the keyboard\n");
        EnableOsccSteering();
        break;

      case control::OsccAction::DISABLE_STEERING:
        printf("Received OSCC_DISALBE_STEERING command from the keyboard\n");
        DisableOsccSteering();
        break;

      case control::OsccAction::ENABLE_THROTTLE:
        printf("Received OSCC_ENABLE_THROTTLE command from the keyboard\n");
        EnableOsccThrottle();
        break;

      case control::OsccAction::DISABLE_THROTTLE:
        printf("Received OSCC_DISALBE_THROTTLE command from the keyboard\n");
        DisableOsccThrottle();
        break;
    }
  }

  // Execute action to transform driving mode
  if (control_command.has_pad_msg() && control_command.pad_msg().has_action()) 
  {
    AINFO << "Canbus received pad msg: "
          << control_command.pad_msg().ShortDebugString() ;

    Chassis::DrivingMode mode = Chassis::COMPLETE_MANUAL;
    switch (control_command.pad_msg().action()) 
    {
      case control::DrivingAction::START: 
        mode = Chassis::COMPLETE_AUTO_DRIVE;
        break;

      case control::DrivingAction::STOP:
      case control::DrivingAction::RESET: 
        // In COMPLETE_MANUAL mode
        break;

      default: 
        AERROR << "No response for this action.";
        break;
    }  

    auto error_code = SetDrivingMode(mode);
    if (error_code != ErrorCode::OK) 
    {
      AERROR << "Failed to set driving mode.";
      return error_code;
    }
  }


  // Throttle(30);
  // Brake(50);
  Throttle(control_command.throttle());
  Brake(control_command.brake());
  // Steer(control_command.steering_target());
  Steer(control_command.steering_target(), 100);
  // Steer(-90);

  // if (driving_mode() == Chassis::COMPLETE_AUTO_DRIVE 
  //     || driving_mode() == Chassis::AUTO_SPEED_ONLY ) 
  // {
  //   // Gear(control_command.gear_location());
  //   Throttle(control_command.throttle());
  //   // Acceleration(control_command.acceleration());
  //   Brake(control_command.brake());
  //   // SetEpbBreak(control_command);
  //   SetLimits();
  // }

  // if (driving_mode() == Chassis::COMPLETE_AUTO_DRIVE 
  //     || driving_mode() == Chassis::AUTO_STEER_ONLY) 
  // {
  //   const double steering_rate_threshold = 1.0;
  //   if (control_command.steering_rate() > steering_rate_threshold) 
  //   { Steer(control_command.steering_target(), control_command.steering_rate());
  //   } 
  //   else {
  //     Steer(control_command.steering_target());
  //   }
  // }

  // if ((driving_mode() == Chassis::COMPLETE_AUTO_DRIVE 
  //         || driving_mode() == Chassis::AUTO_SPEED_ONLY 
  //         || driving_mode() == Chassis::AUTO_STEER_ONLY ) 
  //      && control_command.has_signal() ) 
  // {
  //   SetHorn(control_command);
  //   SetTurningSignal(control_command);
  //   SetBeam(control_command);
  // }

  return ErrorCode::OK;
}

}  // namespace canbus
}  // namespace apollo

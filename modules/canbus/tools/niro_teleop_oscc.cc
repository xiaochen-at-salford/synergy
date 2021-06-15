#include <termios.h>

#include <cstdio>
#include <iostream>
#include <memory>
#include <thread>

#include "cyber/common/log.h"
#include "cyber/common/macros.h"
#include "cyber/cyber.h"
#include "cyber/init.h"
#include "cyber/time/time.h"
#include "modules/canbus/proto/chassis.pb.h"
#include "modules/canbus/vehicle/vehicle_controller.h"
#include "modules/common/adapters/adapter_gflags.h"
#include "modules/common/configs/vehicle_config_helper.h"
#include "modules/common/util/message_util.h"
#include "modules/control/proto/control_cmd.pb.h"
#include "modules/drivers/canbus/proto/oscc_status_cmd.pb.h"

// gflags
DEFINE_double(throttle_inc_delta, 2.0, "throttle pedal command delta percentage.");
DEFINE_double(brake_inc_delta, 2.0, "brake pedal delta percentage");
DEFINE_double(steer_inc_delta, 2.0, "steer delta percentage");
// TODO(ALL) : switch the acceleration cmd or pedal cmd
// default : use pedal cmd
DEFINE_bool(use_acceleration, false, "switch to use acceleration instead of throttle pedal and brake pedal" );

namespace {
  
// typedef ::apollo::drivers::canbus::OsccStatusCommand::Type OsccStatusCommandType;

using apollo::canbus::Chassis;
using apollo::common::VehicleSignal;
using apollo::control::ControlCommand;
using apollo::cyber::CreateNode;
using apollo::cyber::Reader;
using apollo::cyber::Time;
using apollo::cyber::Writer;
typedef apollo::drivers::canbus::OsccStatusCommand_Type OsccStatusCommandType;

const uint32_t KEYCODE_O = 0x4F;  // '0'

// Keys for OSCC status
const uint32_t KEYCODE_OSCC_ENABLE = 0x4F;  // 'O'
const uint32_t KEYCODE_OSCC_DISABLE = 0x6F;  // 'o'
const uint32_t KEYCODE_BRAKE_ENABLE = 0x42;  // 'B'
const uint32_t KEYCODE_BRAKE_DISABLE = 0x62;  // 'b'
const uint32_t KEYCODE_STEERING_ENABLE = 0x53;  // 'S'
const uint32_t KEYCODE_STEERING_DISABLE = 0x73;  // 's'
const uint32_t KEYCODE_THROTTLE_ENABLE = 0x54;  // 'T'
const uint32_t KEYCODE_THROTTLE_DISABLE = 0x74;  // 't'

const uint32_t KEYCODE_HELP = 0x68;   // 'h'
const uint32_t KEYCODE_HELP2 = 0x48;  // 'H'

class Teleop {
 public:
  Teleop() 
  {
    ResetControlCommand();
    node_ = CreateNode("teleop");
  }

  static void PrintKeycode() 
  {
    system("clear");
    printf("=====================    KEYBOARD MAP   ===================\n");
    printf("HELP:               [%c]     |\n", KEYCODE_HELP);
    printf("Set KIA Niro Mode:                       \n");
    printf("                     0 Manul\n");
    printf("                     1 Auto\n");
    printf("                     2 Speed Only\n");
    printf("                     3 Steering Only\n");
    printf("\n-----------------------------------------------------------\n");
    printf("Set OSCC Status: ");
    printf("                     B/b Enable/Disable Brake\n");
    printf("                     S/s Enable/Disable Steering\n");
    printf("                     T/t Enable/Disable Steering\n");
    printf("\n-----------------------------------------------------------\n");
    printf("Exit: Ctrl + C, then press enter to normal terminal\n");
    printf("===========================================================\n");
  }

  void KeyboardLoopThreadFunc() 
  {
    char c = 0;
    // int32_t level = 0;
    // double brake = 0.0; // [0, 100]
    // double throttle = 0.0; // [0, 100]
    // double acc = 0;
    // double dec = 0;
    // double steering = 0; // [-100, 100]
    struct termios cooked_;
    struct termios raw_;
    int32_t kfd_ = 0;
    // Chassis::GearPosition gear = Chassis::GEAR_INVALID;
    ControlCommand &control_command_ = control_command();
    apollo::common::VehicleParam vehicle_params_ =
        apollo::common::VehicleConfigHelper::Instance(
            )->GetConfig().vehicle_param() ;

    // get the console in raw mode
    tcgetattr(kfd_, &cooked_);
    std::memcpy(&raw_, &cooked_, sizeof(struct termios));
    raw_.c_lflag &= ~(ICANON | ECHO);
    // Setting a new line, then end of file
    raw_.c_cc[VEOL] = 1;
    raw_.c_cc[VEOF] = 2;
    tcsetattr(kfd_, TCSANOW, &raw_);
    puts("Synergy KIA Niro Teleop:\nReading from keyboard now.");
    puts("---------------------------");
    puts("Use arrow keys to drive the car.");
    while (IsRunning()) 
    {
      // get the next event from the keyboard
      if (read(kfd_, &c, 1) < 0) 
      {
        perror("read():");
        exit(-1);
      }
      AINFO << "control command : "
            << control_command_.ShortDebugString().c_str();
      switch (c) 
      {
        case KEYCODE_OSCC_ENABLE: 
          control_command_.mutable_oscc_status_cmd(
              )->set_cmd(OsccStatusCommandType::OsccStatusCommand_Type_ENABLE_ALL);
          break;
        
        case KEYCODE_OSCC_DISABLE:
          control_command_.mutable_oscc_status_cmd(
              )->set_cmd(OsccStatusCommandType::OsccStatusCommand_Type_DISABLE_ALL);
          break;
        
        case KEYCODE_BRAKE_ENABLE:
          control_command_.mutable_oscc_status_cmd(
              )->set_cmd(OsccStatusCommandType::OsccStatusCommand_Type_ENABLE_BRAKE);
          break;
        
        case KEYCODE_BRAKE_DISABLE:
          control_command_.mutable_oscc_status_cmd(
              )->set_cmd(OsccStatusCommandType::OsccStatusCommand_Type_DISABLE_BRAKE);
          break;
        
        case KEYCODE_STEERING_ENABLE:
          control_command_.mutable_oscc_status_cmd(
              )->set_cmd(OsccStatusCommandType::OsccStatusCommand_Type_ENABLE_STEERING);
          break;

        case KEYCODE_STEERING_DISABLE:
          control_command_.mutable_oscc_status_cmd(
              )->set_cmd(OsccStatusCommandType::OsccStatusCommand_Type_DISABLE_STEERING);
          break;
        
        case KEYCODE_THROTTLE_ENABLE:
          control_command_.mutable_oscc_status_cmd(
              )->set_cmd(OsccStatusCommandType::OsccStatusCommand_Type_ENABLE_THROTTLE);
          break;
        
        case KEYCODE_THROTTLE_DISABLE:
          control_command_.mutable_oscc_status_cmd(
              )->set_cmd(OsccStatusCommandType::OsccStatusCommand_Type_DISABLE_THROTTLE);
          break;

        case KEYCODE_HELP:
        case KEYCODE_HELP2:
          PrintKeycode();
          break;

        default:
          printf("%X\n", c);
          break;
      }
      AINFO << "control command after switch : "
            << control_command_.ShortDebugString().c_str() ;
    }  // keyboard_loop big while

    tcsetattr(kfd_, TCSANOW, &cooked_);
    AINFO << "keyboard_loop thread quited.";
    return;
  }  // end of keyboard loop thread

  ControlCommand &control_command() { return control_command_; }

  double GetCommand(double val, double inc) 
  {
    val += inc;
    if (val > 100.0) 
    { val = 100.0; } 
    else if (val < -100.0) 
    { val = -100.0; }
    return val;
  }

  void Send() 
  {
    apollo::common::util::FillHeader("control", &control_command_);
    control_command_writer_->Write(control_command_);
    ADEBUG << "Control Command send OK:" << control_command_.ShortDebugString();
  }

  void ResetControlCommand() 
  { control_command_.Clear(); }

  void OnChassis(const Chassis &chassis) { Send(); }

  int32_t Start() 
  {
    if (is_running_) 
    {
      AERROR << "Already running.";
      return -1;
    }
    is_running_ = true;
    chassis_reader_ = node_->CreateReader<Chassis>(
        FLAGS_chassis_topic, [this](const std::shared_ptr<Chassis> &chassis) 
        { OnChassis(*chassis); });
    control_command_writer_ = node_->CreateWriter<ControlCommand>(FLAGS_control_command_topic);
    keyboard_thread_.reset(new std::thread([this] { KeyboardLoopThreadFunc(); }));
    if (keyboard_thread_ == nullptr) 
    {
      AERROR << "Unable to create can client receiver thread.";
      return -1;
    }
    return 0;
  }

  void Stop() 
  {
    if (is_running_) 
    {
      is_running_ = false;
      if (keyboard_thread_ != nullptr && keyboard_thread_->joinable()) 
      {
        keyboard_thread_->join();
        keyboard_thread_.reset();
        AINFO << "Teleop keyboard stopped [ok].";
      }
    }
  }

  bool IsRunning() const { return is_running_; }

 private:
  std::unique_ptr<std::thread> keyboard_thread_;
  std::shared_ptr<Reader<Chassis>> chassis_reader_;
  std::shared_ptr<Writer<ControlCommand>> control_command_writer_;
  ControlCommand control_command_;
  bool is_running_ = false;
  std::shared_ptr<apollo::cyber::Node> node_;
};

} // namespace

int main(int32_t argc, char **argv) 
{
  apollo::cyber::Init(argv[0]);
  FLAGS_alsologtostderr = true;
  FLAGS_v = 3;

  google::ParseCommandLineFlags(&argc, &argv, true);

  Teleop teleop;

  if (teleop.Start() != 0) 
  {
    AERROR << "Teleop start failed.";
    return -1;
  }
  Teleop::PrintKeycode();
  apollo::cyber::WaitForShutdown();
  teleop.Stop();
  AINFO << "Teleop exit done.";
  return 0;
}

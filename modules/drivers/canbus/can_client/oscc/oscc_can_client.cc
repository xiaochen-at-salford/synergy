
#include "modules/drivers/canbus/can_client/oscc/oscc_can_client.h"

#include "absl/strings/str_cat.h"

#include <fcntl.h>

namespace apollo {
namespace drivers {
namespace canbus {
namespace can {

// using apollo::common::ErrorCode;

//TODO(xiaochen):
bool OsccCanClient::Init(const CANCardParameter &parameter)
{
  if (!parameter.has_channel_id()) 
  {
    AERROR << "Init CAN failed: parameter does not have channel id. "
              "The parameter is "
           << parameter.DebugString();
    return false;
  }

  port_ = parameter.channel_id();
  interface_ = parameter.interface();
  auto num_ports = parameter.num_ports();
  if (port_ > static_cast<int32_t>(num_ports) || port_ < 0) 
  {
    AERROR << "Can port number [" << port_ << "] is out of range [0, "
           << num_ports << ") !";
    return false;
  }

  tv_send_.tv_sec = 0;
  tv_send_.tv_usec = 100;
  tv_recv_.tv_sec = 0;
  tv_recv_.tv_usec = 100;

  return true;
}

OsccCanClient::~OsccCanClient() 
{
  if (dev_send_ || dev_recv_) 
  { Stop(); }
}

ErrorCode OsccCanClient::Start() 
{
  if (is_started_)
  { return ErrorCode::OK; }

  if (init_can_channel(&dev_send_, can_send_port_, &tv_send_) != ErrorCode::OK)
  { 
    AERROR << "Failed to initialise OSCC CAN Sender";
    return ErrorCode::CAN_CLIENT_ERROR_BASE;
  }

  if (init_can_channel(&dev_recv_, can_recv_port_, &tv_recv_) != ErrorCode::OK)
  {
    AERROR << "Failed to initialise OSCC CAN Receiver";
    return ErrorCode::CAN_CLIENT_ERROR_BASE;
  }

  is_started_ = true;

  return ErrorCode::OK;
}

void OsccCanClient::Stop() 
{
  if (is_started_)
  {
    is_started_ = false;
    int result = 0;
    result = close(dev_send_);
    if (result < 0)
    { AERROR << "OSCC CAN Sender error code: " << result << ", " << GetErrorString(result); }
    else
    { AINFO << "Close OSCC CAN sender socket ok"; }

    result = close(dev_recv_);
    if (result < 0)
    { AERROR << "OSCC CAN receiver error code: " << result << ", " << GetErrorString(result); }
    else
    { AINFO << "Close OSCC CAN receiver socket ok"; }
  }
}

ErrorCode OsccCanClient::Send(const std::vector<CanFrame> &frames, int32_t *const frame_num) 
{
  CHECK_NOTNULL(frame_num);
  CHECK_EQ(frames.size(), static_cast<size_t>(*frame_num));

  if (!is_started_) 
  {
    AERROR << "OSCC can client has not been initiated! Please init first!";
    return ErrorCode::CAN_CLIENT_ERROR_SEND_FAILED;
  }

  for (size_t i = 0; i < frames.size() && i < MAX_CAN_SEND_FRAME_LEN; ++i) 
  {
    if (frames[i].len > CANBUS_MESSAGE_LENGTH || frames[i].len < 0) 
    {
      AERROR << "frames[" << i << "].len = " << frames[i].len
             << ", which is not equal to can message data length ("
             << CANBUS_MESSAGE_LENGTH << ").";
      return ErrorCode::CAN_CLIENT_ERROR_SEND_FAILED;
    }

    // if (send_frames_[i].can_id == 114 ) 
    // {
    //   // typedef struct {
    //   //   uint8_t magic[2]; /*!< Magic number identifying CAN frame as from OSCC.
    //   //                      *   Byte 0 should be \ref OSCC_MAGIC_BYTE_0.
    //   //                      *   Byte 1 should be \ref OSCC_MAGIC_BYTE_1. */
    //   // float pedal_command; /* Brake Request 0.0 to 1.0 where 1.0 is 100% */
    //   // uint8_t reserved[2]; /*!< Reserved. */
    //   // } oscc_brake_command_s;

    //   // oscc_brake_command_s frame;
    //   // frame.magic[0] = 0x05;
    //   // frame.magic[1] = 0xCC;
    //   // frame.pedal_command = static_cast<float>(0.5);

    //   // std::memcpy(&send_frames_[i].data, &frame, sizeof(frame));
    //   send_frames_[i].data [0] = 0x05;
    //   send_frames_[i].data [1] = 0xCC;
    //   float x = 0.5;
    //   std::memcpy(&(send_frames_[i].data[2]), &x, sizeof(x));
    //   write(dev_send_, &send_frames_[i], sizeof(send_frames_[i]));
    //   continue;
    // }

    // // Throttle command
    // if (send_frames_[i].can_id == 146 ) 
    // {
    //   // typedef struct {
    //   //   uint8_t magic[2]; /*!< Magic number identifying CAN frame as from OSCC.
    //   //                      *   Byte 0 should be \ref OSCC_MAGIC_BYTE_0.
    //   //                      *   Byte 1 should be \ref OSCC_MAGIC_BYTE_1. */
    //   //   float throttle_command; /* Brake Request 0.0 to 1.0 where 1.0 is 100% */
    //   //   uint8_t reserved[2]; /*!< Reserved. */
    //   // } oscc_throttle_command_s;

    //   // oscc_throttle_command_s cmd_frame;
    //   // cmd_frame.magic[0] = 0x05;
    //   // cmd_frame.magic[1] = 0xCC;
    //   // cmd_frame.throttle_command = static_cast<float>(0.7);

    //   // std::memcpy(send_frames_[i].data, &cmd_frame, sizeof(cmd_frame));

    //   send_frames_[i].data [0] = 0x05;
    //   send_frames_[i].data [1] = 0xCC;
    //   float x = 0.3;
    //   std::memcpy(&(send_frames_[i].data[2]), &x, sizeof(x));
    //   write(dev_send_, &send_frames_[i], sizeof(send_frames_[i]));
    //   continue;
    // }

    send_frames_[i].can_id = frames[i].id;
    send_frames_[i].can_dlc = frames[i].len;
    std::memcpy(send_frames_[i].data, frames[i].data, frames[i].len);

    // Synchronous transmission of CAN messages
    int result = static_cast<int>(write(dev_send_, &send_frames_[i], sizeof(send_frames_[i])));
    if (result <= 0) 
    {
      AERROR << "send message failed, error code: " << result;
      return ErrorCode::CAN_CLIENT_ERROR_BASE;
    }
 }

 return ErrorCode::OK;
}

ErrorCode OsccCanClient::Receive(std::vector<CanFrame> *const frames, int32_t *const frame_num)
{
  if (!is_started_) 
  {
    AERROR << "OSCC can client is not init! Please init first!";
    return ErrorCode::CAN_CLIENT_ERROR_RECV_FAILED;
  }

  if (*frame_num > MAX_CAN_RECV_FRAME_LEN || *frame_num < 0) 
  {
    AERROR << "recv can frame num not in range[0, " << MAX_CAN_RECV_FRAME_LEN
           << "], frame_num:" << *frame_num;
    // TODO(Authors): check the difference of returning frame_num/error_code
    return ErrorCode::CAN_CLIENT_ERROR_FRAME_NUM;
  }

  //TODO(xiaochen): Fix "receive message failed"
  for (int32_t i = 0; i<*frame_num && i<MAX_CAN_RECV_FRAME_LEN; ++i) 
  {
    if (dev_recv_ <= 0)
    { 
      AERROR << "CAN recv sock has not been initilised." 
             <<  "[recv socket]: " << dev_recv_;
    }
    CanFrame cf;
  
    // struct can_frame rx_frame;

    // results := received number of bytes
    // It is okay to have failed results, i.e. "-1", occasionally, as this 
    // is observed in OSCC driver too. 
    read(dev_recv_, &recv_frames_[i], sizeof(recv_frames_[i]));

    // TODO(xiaochen): Delete the codes below   
    // if (result < 0) 
    // {
    //   AERROR << "receive message failed, error code: " << result
    //          << ", recv socket: " << dev_recv_
    //          << ", send socket: " << dev_send_;
             
    //   return ErrorCode::CAN_CLIENT_ERROR_BASE;
    // }
    // else
    // { 
    //   // printf("OSCC Read worked!");
    // }

    if (recv_frames_[i].can_dlc > CANBUS_MESSAGE_LENGTH 
        || recv_frames_[i].can_dlc < 0 ) 
    {
      AERROR << "recv_frames_[" << i
             << "].can_dlc = " << recv_frames_[i].can_dlc
             << ", which is not equal to can message data length ("
             << CANBUS_MESSAGE_LENGTH << ")." ;
      return ErrorCode::CAN_CLIENT_ERROR_RECV_FAILED;
    }
    cf.id = recv_frames_[i].can_id;
    cf.len = recv_frames_[i].can_dlc;
    std::memcpy(cf.data, recv_frames_[i].data, recv_frames_[i].can_dlc);
    frames->push_back(cf);
  }
  return ErrorCode::OK;
}

std::string OsccCanClient::GetErrorString(const int32_t /*status*/) 
{ return ""; }

//TODO:
// ErrorCode init_can_channel(int &dev_handler, int can_channel, struct timeval *tv)
ErrorCode OsccCanClient::init_can_channel(int *dev_handler, int port, struct timeval *tv)
{
  int result = UNINITIALIZED_SOCKET; 
  // struct sockaddr_can addr;
  //FIXIT: ifr can0 can1
  struct ifreq ifr;
  memset(&ifr, 0, sizeof(ifr));

  // Sender CAN
  *dev_handler = socket(PF_CAN, SOCK_RAW, CAN_RAW);
  if (*dev_handler < 0)
  {
    AERROR << "open device error code [" << dev_handler << "]: ";
    return ErrorCode::CAN_CLIENT_ERROR_BASE;
  }

  std::string interface_prefix = "can";
  const std::string can_name = absl::StrCat(interface_prefix, port);
  strncpy(ifr.ifr_name, can_name.c_str(), IFNAMSIZ);
  strncpy(ifr.ifr_name, can_name.c_str(), IFNAMSIZ);
  result = ioctl(*dev_handler, SIOCGIFINDEX, &ifr); 
  if (result < 0)
  { perror( "Finding CAN index failed:" ); }
  else
  { AINFO << "can name: " << can_name; }

  if (tv != nullptr)
  { 
    result = setsockopt(*dev_handler, 
                        SOL_SOCKET, 
                        SO_RCVTIMEO, 
                        tv, sizeof(struct timeval) );

    if (result < 0)
    { 
      AERROR << "Time-out parameter not set.";
      return ErrorCode::CAN_CLIENT_ERROR_BASE;
    } 
  }

  struct sockaddr_can can_address;

  memset(&can_address, 0, sizeof(can_address));
  can_address.can_family = AF_CAN;
  can_address.can_ifindex = ifr.ifr_ifindex;

  result = bind(*dev_handler, 
                (struct sockaddr*) &can_address, 
                sizeof(can_address) );
  if (result < 0)
  {
    AERROR << "Failed to bind socket";
    close(*dev_handler);
    *dev_handler = UNINITIALIZED_SOCKET;
    return ErrorCode::CAN_CLIENT_ERROR_BASE;
  }

  return ErrorCode::OK;  
}

}  // namespace can
}  // namespace canbus
}  // namespace drivers
}  // namespace apollo

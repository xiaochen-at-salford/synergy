
#include "modules/drivers/canbus/can_client/socket/oscc_can_client_raw.h"

#include "absl/strings/str_cat.h"

namespace apollo {
namespace drivers {
namespace canbus {
namespace can {

using apollo::common::ErrorCode;

//TODO:
bool OsccCanClient::Init(const CANCardParameter &parameter)
{
  if (!parameter.has_channel_id()) 
  {
    AERROR << "Init CAN failed: parameter does not have channel id. The "
              "parameter is "
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
  return true;
}

OsccCanClient::~OsccCanClient() 
{
  if (dev_handler_) 
  { Stop(); }
}


ErrorCode OsccCanClient::Start() 
{
  if (init_can_channel(dev_send_, can_channel_send_, tv_send_) != ErrorCode::OK)
  { 
    AERROR << "Failed to initialise OSCC CAN Sender";
    return ErrorCode::CAN_CLIENT_ERROR_BASE;
  }

  if (init_can_channel(dev_recv_, can_channel_recev_, tv_recv_) != ErrorCode::OK)
  {
    AERROR << "Failed to initialise OSCC CAN Receiver";
    return ErrorCode::CAN_CLIENT_ERROR_BASE;
  }

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
    { AINFO << "Close OSCC CAN sender socket ok"};

    result = close(dev_recv_);
    if (result < 0)
    { AERROR << "OSCC CAN receiver error code: " << result << ", " << GetErrorString(result); }
    else
    { AINFO << "Close OSCC CAN receiver socket ok"};
  }
}

ErrorCode OsccCanClient::Send(const std::vector<CanFrame> &frames, int32_t *const frame_num ) 
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

    send_frames_[i].can_id = frames[i].id;
    send_frames_[i].can_dlc = frames[i].len;
    std::memcpy(send_frames_[i].data, frames[i].data, frames[i].len);

    // Synchronous transmission of CAN messages
    int result = static_cast<int>(write(dev_handler_, &send_frames_[i], sizeof(send_frames_[i])));
    if (result <= 0) 
    {
      AERROR << "send message failed, error code: " << ret;
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

  for (int32_t i = 0; i<*frame_num && i<MAX_CAN_RECV_FRAME_LEN; ++i) 
  {
    CanFrame cf;
    auto result = read(dev_handler_, &recv_frames_[i], sizeof(recv_frames_[i]));

    if (result < 0) 
    {
      AERROR << "receive message failed, error code: " << ret;
      return ErrorCode::CAN_CLIENT_ERROR_BASE;
    }

    if (recv_frames_[i].can_dlc>CANBUS_MESSAGE_LENGTH || recv_frames_[i].can_dlc<0) 
    {
      AERROR << "recv_frames_[" << i
             << "].can_dlc = " << recv_frames_[i].can_dlc
             << ", which is not equal to can message data length ("
             << CANBUS_MESSAGE_LENGTH << ").";
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
{
  return "";
}

//TODO:
ErrorCode init_can_socket(int &dev_handler, int can_channel, struct timeval *tv)
{
  result = Uninitialized_Socket; 
  struct sockaddr_can addr;
  struct ifreq ifr;
  memset(&ifr, 0, sizeof(ifr));

  // Sender CAN
  dev_handler = socket(PF_CAN, SOCK_RAW, CAN_RAW);
  if (dev_handle < 0)
  {
    AERROR << "open device error code [" << dev_handler << "]: ";
    return ErrorCode::CAN_CLIENT_ERROR_BASE;
  }

  strncpy(ifr.ifr_name, can_channel, IFNAMSIZ);
  result = ioctl(dev_handler, SIOCGIFINDEX, &ifr); 
  if (res < 0)
  { perror( "Finding CAN index failed:" ); }   

  if (&tv!=nullptr)
  {
    AERROR << "Time-out parameter not set."
    return ErrorCode::CAN_CLIENT_ERROR_BASE;
  }

  result = setsockopt(dev_handler, SOL_SOCKET, S0_RCVTIME0, tv, sizeof(struct timeval));
  if (result < 0)
  { perror('Setting timeout failed:')};

  struct sockaddr_can can_address;

  memset(&can_address, 0, sizeof(can_address));
  can_address.can_family = AF_CAN;
  can_address.can_ifindex = ifr.ifr_ifindex;

  result = bind(dev_handler, (struct sockaddr*) &can_address, sizeof(can_address)             );
  if (result < 0)
  {
    AERROR << "Failed to bind socket";
    close(dev_handler);
    dev_handler = Uninitalised_Socket;
    return ErrorCode::CAN_CLIENT_ERROR_BASE;
  }
  
  return ErrorCode::OK;  
}


}  // namespace can
}  // namespace canbus
}  // namespace drivers
}  // namespace apollo

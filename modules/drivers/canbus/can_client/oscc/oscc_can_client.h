#pragma once

#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "modules/common/proto/error_code.pb.h"
#include "modules/drivers/canbus/proto/can_card_parameter.pb.h"

#include "gflags/gflags.h"
#include "modules/drivers/canbus/can_client/can_client.h"
#include "modules/drivers/canbus/common/canbus_consts.h"

using apollo::common::ErrorCode;

namespace apollo {
namespace drivers {
namespace canbus {
namespace can {

#define UNINITIALIZED_SOCKET -1
#define CAN_MESSAGE_TIMEOUT 100

class OsccCanClient : public CanClient {
 public:
  bool Init(const CANCardParameter &parameter) override;

  virtual ~OsccCanClient();

  ErrorCode Start() override;

  void Stop() override;

  ErrorCode Send(const std::vector<CanFrame> &frames, int32_t *const frame_num) override;

  ErrorCode Receive(std::vector<CanFrame> *const frames, int32_t *const frame_num) override;

  std::string GetErrorString(const int32_t status) override;

  ErrorCode init_can_channel(int *dev_handler, int port, struct timeval *tv);

 private:
  struct timeval tv_send_;
  struct timeval tv_recv_;
  int dev_send_ = UNINITIALIZED_SOCKET;
  int dev_recv_ = UNINITIALIZED_SOCKET;
  int can0_sock_ = UNINITIALIZED_SOCKET;
  int can1_sock_ = UNINITIALIZED_SOCKET;
  const int can_send_port_ = 0; // "can0"
  const int can_recv_port_ = 1; // "can1"
  CANCardParameter::CANChannelId port_;
  CANCardParameter::CANInterface interface_;
  can_frame send_frames_[MAX_CAN_SEND_FRAME_LEN];
  can_frame recv_frames_[MAX_CAN_RECV_FRAME_LEN];
};

}  // namespace can
}  // namespace canbus
}  // namespace drivers
}  // namespace apollo
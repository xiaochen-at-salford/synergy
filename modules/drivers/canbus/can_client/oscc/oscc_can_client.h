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

namespace apollo {
namespace drivers {
namespace canbus {
namespace can {

const int Uninitialized_Socket = -1;

class OsccCanClientRaw : public CanClient {
 public:
  bool Init(const CANCardParameter &parameter) override;

  virtual ~OsccCanClientRaw();

  apollo::common::ErrorCode Start() override;

  void Stop() override;

  apollo::common::ErrorCode Send(const std::vector<CanFrame> &frames,
                                 int32_t *const frame_num ) override;

  apollo::common::ErrorCode Receive(std::vector<CanFrame> *const frames,
                                    int32_t *const frame_num ) override;

  std::string GetErrorString(const int32_t status) override;

 private:
  apollo::common::ErrorCode init_can_socket(int &dev_handler, int can_channel, struct timeval &tv);

  struct timeval tv_send_;
  struct timeval tv_recv_;
  int dev_send_ = Uninitialized_Socket;
  int dev_recv_ = Uninitialized_Socket;
  int can_channel_send_ = 0;
  int can_channel_recv_ = 1;
  CANCardParameter::CANChannelId port_;
  CANCardParameter::CANInterface interface_;
  can_frame send_frames_[MAX_CAN_SEND_FRAME_LEN];
  can_frame recv_frames_[MAX_CAN_RECV_FRAME_LEN];
};

}  // namespace can
}  // namespace canbus
}  // namespace drivers
}  // namespace apollo
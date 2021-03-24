#pragma once

#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/message_manager.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::MessageManager;

class NMiroMessageManager 
  : public MessageManager<::apollo::canbus::ChassisDetail> 
{
 public:
  NiroMessageManager();
  virtual ~NiroMessageManager();
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
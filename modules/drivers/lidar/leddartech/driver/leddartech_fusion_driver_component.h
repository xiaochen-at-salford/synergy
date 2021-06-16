#pragma once

#include <memory>
#include <string>
#include <thread>

#include "modules/drivers/lidar/leddartech/proto/config.pb.h"
#include "modules/drivers/lidar/leddartech/proto/leddartech.pb.h"

#include "cyber/cyber.h"
#include "modules/drivers/lidar/leddartech/driver/driver.h"

namespace apollo {
namespace drivers {
namespace leddartech {

using apollo::cyber::Component;
using apollo::cyber::Reader;
using apollo::cyber::Writer;
using apollo::drivers::leddartech::LeddartechScan;

class LeddartechFusionDriverComponent : 
    public Component<>, public LeddarCore::LdObject {
 public:
  ~LeddartechFusionDriverComponent();

  bool Init() override;

 private:
  void fusion_poll();
  volatile bool running_;
  std::shared_ptr<LeddartechDriver> drv1_;
  std::shared_ptr<LeddartechDriver> drv2_;
  std::shared_ptr<LeddartechDriver> drv3_;
  std::shared_ptr<LeddartechDriver> drv4_;
  std::shared_ptr<std::thread> fusion_thread_;
  std::shared_ptr<apollo::cyber::Writer<LeddartechScanFusion>> writer_;
};

CYBER_REGISTER_COMPONENT(LeddartechFusionDriverComponent)

}  // namespace leddartech
}  // namespace drivers
}  // namespace apollo
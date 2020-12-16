#pragma once

#include <memory>
#include <string>
#include <thread>

#include "cyber/cyber.h"

#include "modules/drivers/lidar/leddartech/driver/driver.h"
#include "modules/drivers/lidar/leddartech/proto/config.pb.h"
#include "modules/drivers/lidar/leddartech/proto/leddartech.pb.h"


namespace apollo {
namespace drivers {
namespace leddartech {

using apollo::cyber::Component;
using apollo::cyber::Reader;
using apollo::cyber::Writer;
using apollo::drivers::leddartech::LeddartechScan;

class LeddartechDriverComponent : public Component<>, public LeddarCore::LdObject{
 public:
    ~LeddartechDriverComponent();
    bool Init() override;
 
 private:
    void device_poll();
    volatile bool running_;
    std::shared_ptr<LeddartechDriver> drv_;
    std::shared_ptr<std::thread> device_thread_;
    std::shared_ptr<apollo::cyber::Writer<LeddartechScan>> writer_;
};

CYBER_REGISTER_COMPONENT(LeddartechDriverComponent)

}  // namespace leddartech
}  // namespace drivers
}  // namespace apollo
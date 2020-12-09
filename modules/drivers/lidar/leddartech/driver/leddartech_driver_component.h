#pragma once

#include <memory>
#include <string>

#include "cyber/cyber.h"

#include "modules/drivers/lidar/leddartech/proto/config.pb.h"
#include "modules/drivers/lidar/leddartech/proto/leddartech.pb.h"


namespace apollo {
namespace drivers {
namespace leddartech {

using apollo::cyber::Component;

class LeddartechDriverComponent : public Component<> {
 public:
    LeddartechDriverComponent();
    ~LeddartechDriverComponent();
    bool Init();
 
 protected:
    Config config_;
    std::string topic_;
    uint64_t basetime_ = 0;
};

CYBER_REGISTER_COMPONENT(LeddartechDriverComponent)

}  // namespace leddartech
}  // namespace drivers
}  // namespace apollo
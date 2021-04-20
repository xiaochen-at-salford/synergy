#pragma once

#include "cyber/cyber.h"
#include "modules/drivers/gnss/proto/an_gnss.pb.h"
#include "modules/localization/proto/imu.pb.h"

namespace apollo {
namespace drivers {
namespace gnss {

using apollo::cyber::Component;
using apollo::cyber::Writer;
using apollo::drivers::gnss::AnGnss;
using apollo::localization::CorrectedImu; 

class AnRelayComponent : public Component<AnGnss> {
 public:
  bool Init() override;

  bool Proc(const std::shared_ptr<AnGnss>& an_gnss) override;

 private:
  std::shared_ptr<Writer<CorrectedImu>> writer_ = nullptr;
};

CYBER_REGISTER_COMPONENT(AnRelayComponent)

} // namespace gnss
} // namespace driver
} // namespace apollo
#pragma once

#include <memory>
#include <string>

#include "modules/drivers/lidar/leddartech/proto/config.pb.h"
#include "modules/drivers/lidar/leddartech/proto/leddartech.pb.h"
#include "modules/drivers/proto/pointcloud.pb.h"

#include "cyber/cyber.h"

namespace apollo {
namespace drivers {
namespace leddartech {

using apollo::cyber::Component;
using apollo::cyber::Reader;
using apollo::cyber::Writer;
using apollo::drivers::PointCloud;
using apollo::drivers::PointXYZIT;
using apollo::drivers::leddartech::LeddartechScan;

class LeddartechParser : public Component<LeddartechScan> {
 public:
  bool Init() override;
  bool Proc(const std::shared_ptr<LeddartechScan>& scan_msg) override;

 private:
  void GeneratePointCloud(const std::shared_ptr<LeddartechScan>& scan_msg,
                          std::shared_ptr<PointCloud> point_cloud);
  void ConvertPointXYZ(const LeddartechPacket& pkt,
                       std::shared_ptr<PointCloud> pc);
  std::shared_ptr<Writer<PointCloud>> writer_;
};

CYBER_REGISTER_COMPONENT(LeddartechParser)
}  // namespace leddartech
}  // namespace drivers
}  // namespace apollo

#include "modules/drivers/lidar/leddartech/parser/leddartech_parser.h"

#include <chrono>

namespace apollo {
namespace drivers {
namespace leddartech {

using apollo::drivers::PointXYZIT;

void LeddartechParser::ConvertPointXYZ(const LeddartechPacket& pkt,
                                       std::shared_ptr<PointCloud> pc) {
  uint16_t value = pkt.flag() & 0x01;
  if (value == 0) {
    return;
  }

  apollo::drivers::PointXYZIT* point_new = pc->add_point();
  // convert LeddarSDK coordinates to apollo's
  point_new->set_x(pkt.z());
  point_new->set_y(-pkt.x());
  point_new->set_z(pkt.y());

  point_new->set_timestamp(pkt.timestamp());
}

void LeddartechParser::GeneratePointCloud(
    const std::shared_ptr<LeddartechScan>& scan_msg,
    std::shared_ptr<PointCloud> point_cloud) {
  size_t packet_size = scan_msg->detections_size();
  point_cloud->mutable_header()->set_frame_id(scan_msg->header().frame_id());
  point_cloud->set_height(1);
  point_cloud->mutable_header()->set_sequence_num(
      scan_msg->header().sequence_num());
  point_cloud->mutable_header()->set_timestamp_sec(
      cyber::Time().Now().ToSecond());
  point_cloud->set_width(point_cloud->point_size());
  for (size_t i = 0; i < packet_size; ++i) {
    ConvertPointXYZ(scan_msg->detections(static_cast<int>(i)), point_cloud);
  }
}

bool LeddartechParser::Init() {
  Config leddartech_config;
  if (!GetProtoConfig(&leddartech_config)) {
    AWARN << "Load config failed, config file" << config_file_path_;
    return false;
  }

  std::cout << leddartech_config.pointcloud_channel() << std::endl;
  writer_ =
      node_->CreateWriter<PointCloud>(leddartech_config.pointcloud_channel());
  if (writer_ == nullptr) {
    std::cout << "create writer" << std::endl;
    AERROR << "create writer:" << leddartech_config.pointcloud_channel();
    return false;
  }

  return true;
}

bool LeddartechParser::Proc(const std::shared_ptr<LeddartechScan>& scan_msg) {
  std::shared_ptr<PointCloud> point_cloud_out = std::make_shared<PointCloud>();
  if (point_cloud_out == nullptr) {
    AWARN << "point_cloud_out is empty";
    return false;
  }
  point_cloud_out->Clear();

  GeneratePointCloud(scan_msg, point_cloud_out);

  if (point_cloud_out == nullptr || point_cloud_out->point().empty()) {
    AWARN << "point cloud is empty";
    return false;
  }

  writer_->Write(point_cloud_out);
  return true;
}

}  // namespace leddartech
}  // namespace drivers
}  // namespace apollo
#pragma once

#include <atomic>
#include <future>
#include <memory>
#include <vector>

#include "cyber/cyber.h"
#include "modules/drivers/camera/proto/config.pb.h"
#include "modules/drivers/proto/sensor_image.pb.h"

#include "modules/drivers/camera/gige_cam.h"

namespace apollo {
namespace drivers {
namespace camera {

using apollo::cyber::Component;
using apollo::cyber::Reader;
using apollo::cyber::Writer;
using apollo::drivers::Image;
using apollo::drivers::camera::config::HikroConfig;

class SynCameraComponent : public Component<> {
 public:
  bool Init() override;
  ~SynCameraComponent();

 private:
  void run();

  std::shared_ptr<Writer<Image>> writer_ = nullptr;
  std::unique_ptr<GigeCam> camera_device_;
  std::shared_ptr<HikroConfig> camera_config_;
  CameraImagePtr raw_image_ = nullptr;
  std::vector<std::shared_ptr<Image>> pb_image_buffer_;
  uint32_t spin_rate_ = 100;
  uint32_t device_wait_ = 2000;
  int index_ = 0;
  int buffer_size_ = 16;
  const int32_t MAX_IMAGE_SIZE = 20 * 1024 * 1024;
  std::future<void> async_result_;
  std::atomic<bool> running_ = {false};
};

CYBER_REGISTER_COMPONENT(SynCameraComponent)

}  // namespace camera
}  // namespace drivers
}  // namespace apollo

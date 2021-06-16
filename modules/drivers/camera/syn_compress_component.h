#pragma once

#include <memory>

#include "cyber/base/concurrent_object_pool.h"
#include "cyber/cyber.h"
#include "modules/drivers/camera/proto/config.pb.h"
#include "modules/drivers/proto/sensor_image.pb.h"

namespace apollo {
namespace drivers {
namespace camera {

using apollo::cyber::Component;
using apollo::cyber::Writer;
using apollo::cyber::base::CCObjectPool;
using apollo::drivers::Image;
using apollo::drivers::camera::config::HikroConfig;

class SynCompressComponent : public Component<Image> {
 public:
  bool Init() override;
  bool Proc(const std::shared_ptr<Image>& image) override;

 private:
  std::shared_ptr<CCObjectPool<CompressedImage>> image_pool_;
  std::shared_ptr<Writer<CompressedImage>> writer_ = nullptr;
  HikroConfig config_;
};

CYBER_REGISTER_COMPONENT(SynCompressComponent)

}  // namespace camera
}  // namespace drivers
}  // namespace apollo

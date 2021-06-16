#pragma once

#include <asm/types.h> /* for videodev2.h */
#include <malloc.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/mem.h>
#include <libswscale/swscale.h>
#include <linux/videodev2.h>
}

#include <libavcodec/version.h>
#if LIBAVCODEC_VERSION_MAJOR < 55
#define AV_CODEC_ID_MJPEG CODEC_ID_MJPEG
#endif

#include <memory>
#include <sstream>
#include <string>

#include "cyber/cyber.h"

#include "modules/drivers/camera/proto/config.pb.h"

#include "boost/asio/ip/address.hpp"
#include "modules/drivers/camera/hikro/MvCameraControl.h"

namespace apollo {
namespace drivers {
namespace camera {

using apollo::drivers::camera::config::HikroConfig;
using apollo::drivers::camera::config::RGB;
using apollo::drivers::camera::config::YUYV;

typedef MV_FRAME_OUT& MvFrameOutRef;

// camera raw image struct
struct CameraImage {
  int width;
  int height;
  int bytes_per_pixel;
  int image_size;
  int is_new;
  int tv_sec;
  int tv_usec;
  char* image;

  ~CameraImage() 
  {
    if (image != nullptr) 
    {
      free(reinterpret_cast<void*>(image));
      image = nullptr;
    }
  }
};

typedef std::shared_ptr<CameraImage> CameraImagePtr;

class GigeCam {
 public:
  GigeCam();
  virtual ~GigeCam();

  virtual bool init(const std::shared_ptr<HikroConfig> &camera_config);

  // user use this function to get camera frame data
  virtual bool poll(const CameraImagePtr &raw_image);

  bool is_grabbing();

  // bool wait_for_device();

//  private:
  /**
   * @brief Create device handle from the network
   * 
   * @return true 
   * @return false 
   */
  bool engage_device();

  bool disengage_device();

  void set_device_config();

  bool open_device();

  bool close_device();

  bool start_grabbing();

  bool stop_grabbing();

  bool read_frame(CameraImagePtr raw_image);

  bool process_image(MV_FRAME_OUT&, CameraImagePtr dest_image);

  bool reconnect();

  bool reengage_device();

  bool reset_device();

 private:

  std::shared_ptr<HikroConfig> config_;
  int pixel_format_;
  bool is_grabbing_;
  uint64_t image_seq_;

  boost::asio::ip::address address_; // For boost
  std::string ip_str_;
  bool found_device_ = false;

  MV_CC_DEVICE_INFO mv_device_info_; // For Hikro
  void *mv_handle_ = nullptr;

  float frame_warning_interval_ = 0.0;
  float device_wait_sec_ = 0.0;
  uint64_t last_nsec_ = 0;
  float frame_drop_interval_ = 0.0;
};

}  // namespace camera
}  // namespace drivers
}  // namespace apollo







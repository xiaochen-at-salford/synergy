#include "modules/drivers/camera/syn_fhd_camera_component.h"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

namespace apollo {
namespace drivers {
namespace camera {

bool SynFhdCameraComponent::Init() 
{
  camera_config_ = std::make_shared<HikroConfig>();
  if (!apollo::cyber::common::GetProtoFromFile(config_file_path_,
                                               camera_config_.get()) ) 
  { return false; }
  AINFO << "GigeCam config: " << camera_config_->DebugString();

  camera_device_.reset(new GigeCam());
  camera_device_->init(camera_config_);
  raw_image_.reset(new CameraImage);

  raw_image_->width = camera_config_->width();
  raw_image_->height = camera_config_->height();
  raw_image_->bytes_per_pixel = camera_config_->bytes_per_pixel();

  device_wait_ = camera_config_->device_wait_ms();
  spin_rate_ = static_cast<uint32_t>((1.0 / camera_config_->spin_rate()) * 1e6);

  if (camera_config_->output_type() == YUYV) 
  { raw_image_->image_size = raw_image_->width * raw_image_->height * 2; } 
  else if (camera_config_->output_type() == RGB) 
  { raw_image_->image_size = raw_image_->width * raw_image_->height * 3; }

  if (raw_image_->image_size > MAX_IMAGE_SIZE) 
  {
    AERROR << "image size is too big ,must less than " << MAX_IMAGE_SIZE
           << " bytes.";
    return false;
  }
  raw_image_->is_new = 0;
  // free memory in this struct desturctor
  raw_image_->image = reinterpret_cast<char*>(calloc(raw_image_->image_size, sizeof(char)));
  if (raw_image_->image == nullptr) 
  {
    AERROR << "system calloc memory error, size:" << raw_image_->image_size;
    return false;
  }

  for (int i = 0; i < buffer_size_; ++i) 
  {
    auto pb_image = std::make_shared<Image>();
    pb_image->mutable_header()->set_frame_id(camera_config_->frame_id());
    pb_image->set_width(raw_image_->width);
    pb_image->set_height(raw_image_->height);
    pb_image->mutable_data()->reserve(raw_image_->image_size);



    if (camera_config_->output_type() == YUYV) 
    {
      pb_image->set_encoding("yuyv");
      pb_image->set_step(2 * raw_image_->width);
    } 
    else if (camera_config_->output_type() == RGB) 
    {
      pb_image->set_encoding("rgb8");
      pb_image->set_step(3 * raw_image_->width);

    }

    pb_image_buffer_.push_back(pb_image);
  }

  writer_ = node_->CreateWriter<Image>(camera_config_->channel_name());

  camera_device_->engage_device();
  camera_device_->open_device();
  camera_device_->start_grabbing();
  async_result_ = cyber::Async(&SynFhdCameraComponent::run, this);
  return true;
}

void SynFhdCameraComponent::run() 
{
  running_.exchange(true);

  while (!cyber::IsShutdown()) {
    // if (!camera_device_->is_grabbing())
    // {
    //   AINFO << "The Gige camera is not grabbing. Start to reconnect...";
    //   camera_device_->reconnect();
    // }

    if (!camera_device_->poll(raw_image_)) 
    {
      AERROR << "camera device poll failed";
      continue;
    }

    cyber::Time image_time(raw_image_->tv_sec, 1000 * raw_image_->tv_usec);
    if (index_ >= buffer_size_) 
    { index_ = 0; }

    auto pb_image = pb_image_buffer_.at(index_++);
    pb_image->mutable_header()->set_timestamp_sec(cyber::Time::Now().ToSecond());
    pb_image->set_measurement_time(image_time.ToSecond());
    pb_image->set_data(raw_image_->image, raw_image_->image_size);

    cv::Mat mat_image(pb_image->height(), pb_image->width(), CV_8UC3,
                      const_cast<char*>(pb_image->data().data()), pb_image->step());

    cv::Mat fhd_image;

    cv::resize(mat_image, fhd_image, cv::Size(1920,1080),0, 0, 1); 

   auto pb_fhd_image = std::make_shared<Image>();
    pb_fhd_image->mutable_header()->set_frame_id(camera_config_->frame_id());
    pb_fhd_image->set_width(1920);
    pb_fhd_image->set_height(1080);
    pb_fhd_image->mutable_data()->reserve(1920*1080);
    pb_fhd_image->set_encoding("rgb8");
    pb_fhd_image->set_step(3 * 1920);
    pb_fhd_image->mutable_header()->set_timestamp_sec(cyber::Time::Now().ToSecond());
    pb_fhd_image->set_measurement_time(image_time.ToSecond());
    pb_fhd_image->set_data(fhd_image.data, 1920*1080*3); 

    writer_->Write(pb_fhd_image);

    // cyber::SleepFor(std::chrono::microseconds(spin_rate_));
  }
}

SynFhdCameraComponent::~SynFhdCameraComponent() 
{
  if (running_.load()) 
  {
    running_.exchange(false);
    async_result_.wait();
  }
}

}  // namespace camera
}  // namespace drivers
}  // namespace apollo

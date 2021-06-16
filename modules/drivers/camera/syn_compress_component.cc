#include "modules/drivers/camera/syn_compress_component.h"

#include <exception>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

namespace apollo {
namespace drivers {
namespace camera {

bool SynCompressComponent::Init() 
{
  if (!GetProtoConfig(&config_)) 
  {
    AERROR << "Parse config file failed: " << ConfigFilePath();
    return false;
  }
  AINFO << "Camera config: \n" << config_.DebugString();
  try 
  {
    image_pool_.reset(new CCObjectPool<CompressedImage>(config_.compress_conf().image_pool_size()));
    image_pool_->ConstructAll();
  } 
  catch (const std::bad_alloc &e) 
  {
    AERROR << e.what();
    return false;
  }

  writer_ = node_->CreateWriter<CompressedImage>(config_.compress_conf().output_channel());
  return true;
}

bool SynCompressComponent::Proc(const std::shared_ptr<Image>& image) 
{
  ADEBUG << "procing compressed";
  auto compressed_image = image_pool_->GetObject();
  compressed_image->mutable_header()->CopyFrom(image->header());
  compressed_image->set_frame_id(image->frame_id());
  compressed_image->set_measurement_time(image->measurement_time());
  compressed_image->set_format(image->encoding() + "; jpeg compressed bgr8");

  std::vector<int> params;
  params.resize(3, 0);
  params[0] = cv::IMWRITE_JPEG_QUALITY;
  params[1] = 95;

  try 
  {
    cv::Mat mat_image(image->height(), image->width(), CV_8UC3,
                      const_cast<char*>(image->data().data()), image->step());
    cv::Mat tmp_mat;
    cv::cvtColor(mat_image, tmp_mat, cv::COLOR_RGB2BGR);
    std::vector<uint8_t> compress_buffer;
    if (!cv::imencode(".jpg", tmp_mat, compress_buffer, params)) 
    {
      AERROR << "cv::imencode (jpeg) failed on input image";
      return false;
    }
    compressed_image->set_data(compress_buffer.data(), compress_buffer.size());
    writer_->Write(compressed_image);
  } 
  catch (std::exception& e) 
  {
    AERROR << "cv::imencode (jpeg) exception :" << e.what();
    return false;
  }
  return true;
}

}  // namespace camera
}  // namespace drivers
}  // namespace apollo

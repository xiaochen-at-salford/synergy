#include <cmath>
#include <string>

#include "modules/drivers/camera/gige_cam.h"
#include "modules/drivers/camera/util.h"

#include <unistd.h>

#include <boost/system/error_code.hpp>
#include <boost/asio/ip/address.hpp>

#include "modules/drivers/camera/hikro/MvCameraControl.h"

#define __STDC_CONSTANT_MACROS

#define CLEAR(x) memset(&(x), 0, sizeof(x))

namespace apollo {
namespace drivers {
namespace camera {

GigeCam::GigeCam()
  : is_grabbing_(false),
    image_seq_(0),
    device_wait_sec_(2),
    last_nsec_(0),
    frame_drop_interval_(0.0)
{}

GigeCam::~GigeCam() 
{
  stop_grabbing();
  close_device();
  disengage_device();
}

bool GigeCam::init(const std::shared_ptr<HikroConfig> &camera_config)
{
  config_ = camera_config;
  
  // Check IP formate
  boost::system::error_code ec;
  
  address_ = boost::asio::ip::address::from_string(camera_config->device_ip(), ec);
  if (ec)
  {
    AERROR << "The device ip: " << camera_config->device_ip() << "has wrong formate.";
    std::cerr << ec.message( ) << std::endl;
    return false;
  }

  // Pxiel format
  if (config_->pixel_format() == "yuyv") 
  { 
    pixel_format_ = V4L2_PIX_FMT_YUYV; 
    MV_CC_SetPixelFormat(mv_handle_, PixelType_Gvsp_YUV422_YUYV_Packed);
  } 
  else 
  {
    AERROR << "Wrong pixel fromat:" << config_->pixel_format()
           << ",must be yuyv";
    return false;
  }

  // Warning when diff with last > 1.5* interval
  frame_warning_interval_ = static_cast<float>(1.5 / config_->frame_rate());

  // Now max fps 30, we use an appox time 0.9 to drop image.
  frame_drop_interval_ = static_cast<float>(0.9 / config_->frame_rate());

  return true;
}

bool GigeCam::poll(const CameraImagePtr &raw_image)
{
  raw_image->is_new = 0;
  // free memory in this struct desturctor
  memset(raw_image->image, 0, raw_image->image_size*sizeof(char));

  int get_new_image = read_frame(raw_image);
  if (!get_new_image) 
  { return false; }

  raw_image->is_new = 1;
  return true;
}

bool GigeCam::is_grabbing()
{ 
  return is_grabbing_; 
}

bool GigeCam::engage_device()
{
  // Find the GigE cameras in the network
  std::string ip_str = config_->device_ip();
  MV_CC_DEVICE_INFO_LIST device_list;
  memset(&device_list, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
  int mv_res = MV_OK;
  mv_res = MV_CC_EnumDevices(MV_GIGE_DEVICE, &device_list);
  if (mv_res != MV_OK)
  {
    printf("Enum Devices fail! nRet [0x%x]\n", mv_res);
    AERROR << "Failed to enume GigE devices";
    return false;
  }
  if (device_list.nDeviceNum > 0)
  {
    for (unsigned int i = 0; i < device_list.nDeviceNum; ++i)
    {
      MV_CC_DEVICE_INFO *device_info = device_list.pDeviceInfo[i];
      if (device_info == nullptr)
      { break; } 

      if (address_.to_v4().to_ulong() == device_info->SpecialInfo.stGigEInfo.nCurrentIp)
      {
        AINFO << "Found the GigE camera: " << address_.to_v4().to_string();
        mv_device_info_ = *device_info;
        ip_str_ = ip_str;
        found_device_ = true;
        // MV_CC_CreateHandle(&mv_handle_, &mv_device_info_);
        MV_CC_CreateHandle(&mv_handle_, device_info);
        break;
      }
    }

    if (!found_device_)
    {
      AERROR << "Not found the GigE camera: " << ip_str;
      return false;
    }
  } 
  else
  { 
    AERROR << "Not found the GigE camera: " << ip_str; 
    return false;
  }



  //TODO(xiaochen-at-sal;ford)

  return true;
}

bool GigeCam::disengage_device()
{
  int mv_res = MV_CC_DestroyHandle(mv_handle_);
  if (mv_res != MV_OK)
  {
    AERROR << "Failed to destory handle"
           << "mv_res: " << mv_res ;
    return false;
  }
  return false; 
}

bool GigeCam::open_device(void)
{
  int mv_res = MV_CC_OpenDevice(mv_handle_);
  if (mv_res != MV_OK)
  {
    AERROR << "Failed to open device "
           << "mv_res: " << mv_res ;
    return false;
  }

  int packet_size = MV_CC_GetOptimalPacketSize(mv_handle_);
  if (packet_size > 0)
  {
    mv_res = MV_CC_SetIntValue(mv_handle_,"GevSCPSPacketSize",packet_size);
    if(mv_res != MV_OK)
    {
      AERROR << "Failed to set packet size "
              << "mv_res: " << mv_res ;
      return false;
    }
  }
  else
  {
    AERROR << "Failed to set packet size "
            << "mv_res: " << mv_res ;
    return false;
  }

  mv_res = MV_CC_SetEnumValue(mv_handle_, "TriggerMode", 0);
  if (mv_res != MV_OK)
  {
    AERROR << "Failed to set trigger mode "
           << "mv_res: " << mv_res ;
    return false;
  }

  mv_res = MV_CC_SetWidth(mv_handle_, config_->width());
  if (mv_res != MV_OK)
  {
    AERROR << "Failed to set image width."
           << "mv_res: " << mv_res ;
  }

  mv_res = MV_CC_SetHeight(mv_handle_, config_->height());
  if (mv_res != MV_OK)
  {
    AERROR << "Failed to set image height."
           << "mv_res: " << mv_res ;
  }

  // mv_res = MV_CC_SetBrightness(mv_handle_, config_.brightness());
  // if (mv_res != MV_OK)
  // {
  //   AERROR << "Failed to set brightness."
  //          << "mv_res: " << mv_res ;
  // }

  mv_res = MV_CC_SetFrameRate(mv_handle_, config_->frame_rate());
  if (mv_res != MV_OK)
  {
    AERROR << "Failed to set frame rate."
           << "mv_res: " << mv_res ;
  }

  // //TODO Gain mode
  mv_res = MV_CC_SetGainMode(mv_handle_, config_->gain_mode());
  MVCC_ENUMVALUE pstValue;
  MV_CC_GetGainMode(mv_handle_, &pstValue);
  // std::cout << "hehe value: " << pstValue.nCurValue << std::endl;

  if (mv_res != MV_OK)
  {
    AERROR << "Failed to set gain mode: " << config_->gain_mode()
           << "mv_res: " << mv_res ;
  }


  // //TODO Gain
  mv_res = MV_CC_SetGain(mv_handle_, config_->gain());
  if (mv_res != MV_OK)
  {
    AERROR << "Failed to set gain value " << config_->gain()
           << " mv_res: " << mv_res ;
  }

  mv_res = MV_CC_SetGammaSelector(mv_handle_, static_cast<const unsigned int>(config_->gain_mode()));
  if (mv_res != MV_OK)
  {
    AERROR << "Failed to set gain value " << config_->gain_mode()
           << " mv_res: " << mv_res ;
  }


 MV_CC_SetBoolValue(mv_handle_, "GammaEnable", config_->gamma_enable());
 if (mv_res != MV_OK)
 {
    AERROR << "Failed to enable gamma with " << config_->gamma_enable() 
           << " mv_res: " << mv_res ;
 }

  return true; 
}

bool GigeCam::close_device(void)
{
  int mv_res = MV_CC_CloseDevice(mv_handle_);
  if (mv_res != MV_OK)
  {
    AERROR << "Failed to stop grabbing "
           << "mv_res: " << mv_res ;
    return false; 
  }
  return true;   
}

bool GigeCam::start_grabbing(void)
{
  if (is_grabbing_)
  { return true; }

  int mv_res = MV_CC_StartGrabbing(mv_handle_);
  if (mv_res != MV_OK)
  {
    AERROR << "Failed to start grabbing "
           << "mv_res: " << mv_res ;

    return false;
  }

  is_grabbing_ = true;
  return true;
}

bool GigeCam::stop_grabbing(void)
{
  if (!is_grabbing_)
  { return true; }

  is_grabbing_ = false;

  int mv_res = MV_CC_StopGrabbing(mv_handle_);
  if (mv_res != MV_OK)
  {
    AERROR << "Stop caputring fail."
           << "mv_res: " << mv_res;
    return false;
  }

  return true;
}

bool PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo)
{
    if (NULL == pstMVDevInfo)
    {
        printf("The Pointer of pstMVDevInfo is NULL!\n");
        return false;
    }
    if (pstMVDevInfo->nTLayerType == MV_GIGE_DEVICE)
    {
        int nIp1 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
        int nIp2 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
        int nIp3 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
        int nIp4 = (pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);

        // ch:打印当前相机ip和用户自定义名字 | en:print current ip and user defined name
        printf("CurrentIp: %d.%d.%d.%d\n" , nIp1, nIp2, nIp3, nIp4);
        printf("UserDefinedName: %s\n\n" , pstMVDevInfo->SpecialInfo.stGigEInfo.chUserDefinedName);
    }
    else if (pstMVDevInfo->nTLayerType == MV_USB_DEVICE)
    {
        printf("UserDefinedName: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName);
        printf("Serial Number: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chSerialNumber);
        printf("Device Number: %d\n\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.nDeviceNumber);
    }
    else
    {
        printf("Not support.\n");
    }

    return true;
}

bool GigeCam::read_frame(CameraImagePtr raw_image)
{
  int mv_res = MV_OK;

  MV_FRAME_OUT frame = {0};
  memset(&frame, 0, sizeof(MV_FRAME_OUT));

  mv_res = MV_CC_GetImageBuffer(mv_handle_, &frame, 1000);
  // if (mv_res == MV_OK)
  // {
  //     printf("Get One Frame: Width[%d], Height[%d], nFrameNum[%d]\n",
  //         frame.stFrameInfo.nWidth, frame.stFrameInfo.nHeight, frame.stFrameInfo.nFrameNum);
  // }
  // else
  // {
  //     printf("No data[0x%x]\n", mv_res);
  // }
  if (mv_res != MV_OK)
  {
    printf("No data[0x%x]\n", mv_res);
    return false;
  }

  if (!process_image(frame, raw_image))
  {
    AERROR << "Failed to process GigeE camera image frame.";
    return false;
  } 

  if (frame.pBufAddr != nullptr)
  {
    mv_res = MV_CC_FreeImageBuffer(mv_handle_, &frame);
    if (mv_res != MV_OK)
    {
      AERROR << "Failed to free image buffer mv_res: " << mv_res;
      return false;
    }
  }

  return true;
}

//TODO(xiaochen): Add YUYV to RGB conversion
bool GigeCam::process_image(MV_FRAME_OUT& frame, CameraImagePtr dest_image)
{
  if (frame.pBufAddr == nullptr || dest_image == nullptr)
  {
    AERROR << "src image or dest image has a nullptr";
    return false;
  }

  if (config_->output_type() == YUYV)
  {
    memcpy(dest_image->image, 
           frame.pBufAddr, 
           dest_image->width * dest_image->height * 2 );
  }
  else if (config_->output_type() == RGB)
  {
    yuyv2rgb_avx((unsigned char*)frame.pBufAddr, 
                 (unsigned char*)dest_image->image,
                 dest_image->width * dest_image->height );
  } 
  else 
  {
    AERROR << "unsupported output format: " << config_->output_type();
    return false;
  }

  return true;
}

//TODO(xiaochen): switch to exception
bool GigeCam::reconnect()
{
  int mv_res = MV_OK;
  int max_reconnect_times = 10;

  while (max_reconnect_times--)
  {
    mv_res &= stop_grabbing();
    sleep(1);
    mv_res &= close_device();
    sleep(1);
    mv_res &= disengage_device();

    if (mv_res != MV_OK)
    { AERROR << "Failed to disengage the Gige camera"; }
    
    mv_res &= engage_device();
    sleep(1);
    mv_res &= open_device();
    sleep(1);
    mv_res &= start_grabbing();
    if (mv_res != MV_OK)
    { AERROR << "Failed to restarting the Gige camera"; }
    
    return true;
  }

  AERROR << "Tried out max reconnection times.";
  return false;   
}

bool GigeCam::reengage_device()
{
  if (is_grabbing_)
  {
    AERROR << "Still grabbing. Close the device firstly.";
    return false;
  }

  if (mv_handle_ == nullptr)
  {
    AERROR << "Not found the device handle";
    return false;
  }  

  if (!disengage_device())
  {
    AERROR << "Failed to disengage the device";
    return false;
  }

  if (!engage_device())
  {
    AERROR << "Failed to engage the device";
    return false;
  }

  return true;
}

bool GigeCam::reset_device()
{
  if (!close_device())
  {
    AERROR << "Failed to uninit device.";
    return false;
  }
  if (!open_device())
  {
    AERROR << "Failed to init device.";
    return false;
  }
  return true;
}

} // namespace camera
} // namespace drivers
} // namespace apollo
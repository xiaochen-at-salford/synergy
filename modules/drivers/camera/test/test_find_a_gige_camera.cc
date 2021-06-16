#include <iostream>
#include <iterator>
#include <algorithm>

#include <boost/system/error_code.hpp>
#include <boost/asio/ip/address.hpp>

#include "modules/drivers/camera/hikro/MvCameraControl.h"

#define UNUSED(x) do { (void)(x);} while (0)

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
        printf("Device Model Name: %s\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chModelName);
        printf("CurrentIp: %d.%d.%d.%d\n" , nIp1, nIp2, nIp3, nIp4);
        printf("UserDefinedName: %s\n\n" , pstMVDevInfo->SpecialInfo.stGigEInfo.chUserDefinedName);
    }
    else if (pstMVDevInfo->nTLayerType == MV_USB_DEVICE)
    {
        printf("Device Model Name: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chModelName);
        printf("UserDefinedName: %s\n\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName);
    }
    else
    {
        printf("Not support.\n");
    }

    return true;
}

int main()
{
  boost::system::error_code ec;
  std::string target_ip_str = "192.168.1.230";
  // auto ip_address = boost::asio::ip::make_address(target_ip_str, ec);
  auto ip_address = boost::asio::ip::address::from_string(target_ip_str, ec);

  UNUSED(ip_address);

  if (ec)
  {
    std::cout << "error" << std::endl;
  }
  else
  {
    std::cout << "no error" << std::endl;
    std::cout << "the target address is: " << ip_address.to_string() << std::endl;
    std::cout << "the target address in ulong format: " << ip_address.to_v4().to_ulong() << std::endl;
  }

  int nRet = MV_OK;
  // void* handle = NULL;
  do 
  {
    MV_CC_DEVICE_INFO_LIST stDeviceList;
    memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

    // enum device
    nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE, &stDeviceList);
    if (MV_OK != nRet)
    {
      printf("MV_CC_EnumDevices fail! nRet [%x]\n", nRet);
      break;
    }

    if (stDeviceList.nDeviceNum > 0)
    {
      for (unsigned int i = 0; i < stDeviceList.nDeviceNum; i++)
      {
        printf("[device %d]:\n", i);
        MV_CC_DEVICE_INFO* pDeviceInfo = stDeviceList.pDeviceInfo[i];
        if (NULL == pDeviceInfo)
        {
          break;
        } 
        PrintDeviceInfo(pDeviceInfo);            
        auto address = pDeviceInfo->SpecialInfo.stGigEInfo.nCurrentIp;
        if (address == ip_address.to_v4().to_ulong())
        {
          std::cout << "Found device: " << ip_address.to_string() << " !!!"
                    << std::endl;
        }
      }  
    } 
    else
    {
      printf("Find No Devices!\n");
      break;
    }
  } while (0);

}
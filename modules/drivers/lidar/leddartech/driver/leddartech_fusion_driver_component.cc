#include "LdConnectionFactory.h"
#include "LdDeviceFactory.h"
#include "LdLjrRecorder.h"
#include "LdPropertyIds.h"
#include "LdRecordPlayer.h"
#include "LdResultEchoes.h"

// Ethernet
#include "LdConnectionInfoEthernet.h"
#include "LdEthernet.h"
#include "LdSensorPixell.h"

// Utils
#include "LtExceptions.h"
#include "LtKeyboardUtils.h"
#include "LtStringUtils.h"
#include "LtTimeUtils.h"
#include "comm/LtComLeddarTechPublic.h"

#include "cyber/cyber.h"
#include "modules/common/util/message_util.h"
#include "modules/drivers/lidar/leddartech/driver/driver.h"
#include "modules/drivers/lidar/leddartech/driver/leddartech_fusion_driver_component.h"

namespace apollo {
namespace drivers {
namespace leddartech {

LeddartechFusionDriverComponent::~LeddartechFusionDriverComponent() {
  if (fusion_thread_->joinable()) 
  {
    fusion_thread_->join();
    drv1_->GetSensor()->Disconnect();
    drv2_->GetSensor()->Disconnect();
    drv3_->GetSensor()->Disconnect();
    drv4_->GetSensor()->Disconnect();
  }
}

//wip...
bool LeddartechFusionDriverComponent::Init() 
{
  FusionConfig fusion_cfg;
  if (!GetProtoConfig(&fusion_cfg)) 
  { return false; }

  // writer_ = node_->CreateWriter<LeddartechScanFusion>(fusion_cfg.scan_channel());
  writer_ = node_->CreateWriter<LeddartechScanFusion>("/apollo/sensor/lidar/leddartech");

  drv1_ = LeddartechDriverFactory::CreateDriver(fusion_cfg.lidar1());
  drv2_ = LeddartechDriverFactory::CreateDriver(fusion_cfg.lidar2());
  drv3_ = LeddartechDriverFactory::CreateDriver(fusion_cfg.lidar3());
  drv4_ = LeddartechDriverFactory::CreateDriver(fusion_cfg.lidar4());

  drv1_->Init();
  drv2_->Init();
  drv3_->Init();
  drv4_->Init();

  // Set device names
  fusion_cfg.mutable_lidar1()->set_device_name(
      drv1_->GetSensor()->GetProperties()
           ->GetTextProperty(LeddarCore::LdPropertyIds::ID_DEVICE_NAME)
           ->Value() );

  fusion_cfg.mutable_lidar2()->set_device_name(
      drv2_->GetSensor()->GetProperties()
           ->GetTextProperty(LeddarCore::LdPropertyIds::ID_DEVICE_NAME)
           ->Value() );

  fusion_cfg.mutable_lidar3()->set_device_name(
      drv3_->GetSensor()->GetProperties()
           ->GetTextProperty(LeddarCore::LdPropertyIds::ID_DEVICE_NAME)
           ->Value() );

  fusion_cfg.mutable_lidar4()->set_device_name(
      drv4_->GetSensor()->GetProperties()
           ->GetTextProperty(LeddarCore::LdPropertyIds::ID_DEVICE_NAME)
           ->Value() );

  // Set serial numbers
  fusion_cfg.mutable_lidar1()->set_serial_number(
      drv1_->GetSensor()->GetProperties()
           ->GetTextProperty(LeddarCore::LdPropertyIds::ID_SERIAL_NUMBER)
           ->Value() );

  fusion_cfg.mutable_lidar2()->set_serial_number(
      drv2_->GetSensor()->GetProperties()
           ->GetTextProperty(LeddarCore::LdPropertyIds::ID_SERIAL_NUMBER)
           ->Value() );

  fusion_cfg.mutable_lidar3()->set_serial_number(
      drv3_->GetSensor()->GetProperties()
           ->GetTextProperty(LeddarCore::LdPropertyIds::ID_SERIAL_NUMBER)
           ->Value() );

  fusion_cfg.mutable_lidar4()->set_serial_number(
      drv4_->GetSensor()->GetProperties()
           ->GetTextProperty(LeddarCore::LdPropertyIds::ID_SERIAL_NUMBER)
           ->Value() );

  // Set HW part numbers
  fusion_cfg.mutable_lidar1()->set_hw_part_number(
      drv1_->GetSensor()->GetProperties()
           ->GetTextProperty(LeddarCore::LdPropertyIds::ID_PART_NUMBER)
           ->Value());

  fusion_cfg.mutable_lidar2()->set_hw_part_number(
      drv2_->GetSensor()->GetProperties()
           ->GetTextProperty(LeddarCore::LdPropertyIds::ID_PART_NUMBER)
           ->Value());

  fusion_cfg.mutable_lidar3()->set_hw_part_number(
      drv3_->GetSensor()->GetProperties()
           ->GetTextProperty(LeddarCore::LdPropertyIds::ID_PART_NUMBER)
           ->Value());

  fusion_cfg.mutable_lidar4()->set_hw_part_number(
      drv4_->GetSensor()->GetProperties()
           ->GetTextProperty(LeddarCore::LdPropertyIds::ID_PART_NUMBER)
           ->Value());

  // Set SW part numbers
  fusion_cfg.mutable_lidar1()->set_sw_part_number(
      drv1_->GetSensor()->GetProperties()
           ->GetTextProperty(LeddarCore::LdPropertyIds::ID_SOFTWARE_PART_NUMBER)
           ->Value() );

  fusion_cfg.mutable_lidar2()->set_sw_part_number(
      drv2_->GetSensor()->GetProperties()
           ->GetTextProperty(LeddarCore::LdPropertyIds::ID_SOFTWARE_PART_NUMBER)
           ->Value() );

  fusion_cfg.mutable_lidar3()->set_sw_part_number(
      drv3_->GetSensor()->GetProperties()
           ->GetTextProperty(LeddarCore::LdPropertyIds::ID_SOFTWARE_PART_NUMBER)
           ->Value() );

  fusion_cfg.mutable_lidar4()->set_sw_part_number(
      drv4_->GetSensor()->GetProperties()
           ->GetTextProperty(LeddarCore::LdPropertyIds::ID_SOFTWARE_PART_NUMBER)
           ->Value() );

  // Set MAC addresses
  fusion_cfg.mutable_lidar1()->set_mac_address(
      drv1_->GetSensor()->GetProperties()
           ->GetBufferProperty(LeddarCore::LdPropertyIds::ID_MAC_ADDRESS)
           ->GetStringValue() );

  fusion_cfg.mutable_lidar2()->set_mac_address(
      drv2_->GetSensor()->GetProperties()
           ->GetBufferProperty(LeddarCore::LdPropertyIds::ID_MAC_ADDRESS)
           ->GetStringValue() );

  fusion_cfg.mutable_lidar3()->set_mac_address(
      drv3_->GetSensor()->GetProperties()
           ->GetBufferProperty(LeddarCore::LdPropertyIds::ID_MAC_ADDRESS)
           ->GetStringValue() );

  fusion_cfg.mutable_lidar4()->set_mac_address(
      drv4_->GetSensor()->GetProperties()
           ->GetBufferProperty(LeddarCore::LdPropertyIds::ID_MAC_ADDRESS)
           ->GetStringValue() );

  running_ = true;
  fusion_thread_ = std::shared_ptr<std::thread>(new std::thread(
      std::bind(&LeddartechFusionDriverComponent::fusion_poll, this) ) );
  fusion_thread_->detach();

  return true;
}

void LeddartechFusionDriverComponent::fusion_poll() 
{
  while (!apollo::cyber::IsShutdown()) 
  {
    std::shared_ptr<LeddartechScanFusion> fusion_scan = 
        std::make_shared<LeddartechScanFusion>();
    bool ret1 = drv1_->Poll(fusion_scan->mutable_segment1());
    bool ret2 = drv2_->Poll(fusion_scan->mutable_segment2());
    bool ret3 = drv3_->Poll(fusion_scan->mutable_segment3());
    bool ret4 = drv4_->Poll(fusion_scan->mutable_segment4());

    if (ret1 && ret2 && ret3 && ret4) 
    {
      // common::util::FillHeader("leddartech", fusion_scan.get());
      writer_->Write(fusion_scan);
    } 
    else 
    { AWARN << "device poll failed"; }
  }

  AERROR << "LeddartechDriver thread exit";

  running_ = false;
}

}  // namespace leddartech
}  // namespace drivers
}  // namespace apollo
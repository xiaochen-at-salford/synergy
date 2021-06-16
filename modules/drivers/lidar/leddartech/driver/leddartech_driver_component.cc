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
#include "modules/drivers/lidar/leddartech/driver/leddartech_driver_component.h"

namespace apollo {
namespace drivers {
namespace leddartech {

LeddartechDriverComponent::~LeddartechDriverComponent() {
  if (device_thread_->joinable()) {
    device_thread_->join();
    drv_->GetSensor()->Disconnect();
  }
}
bool LeddartechDriverComponent::Init() {
  Config ld_cfg;
  if (!GetProtoConfig(&ld_cfg)) {
    return false;
  }

  writer_ = node_->CreateWriter<LeddartechScan>(ld_cfg.scan_channel());
  drv_ = LeddartechDriverFactory::CreateDriver(ld_cfg);

  drv_->Init();

  ld_cfg.set_device_name(
      drv_->GetSensor()
          ->GetProperties()
          ->GetTextProperty(LeddarCore::LdPropertyIds::ID_DEVICE_NAME)
          ->Value());

  ld_cfg.set_serial_number(
      drv_->GetSensor()
          ->GetProperties()
          ->GetTextProperty(LeddarCore::LdPropertyIds::ID_SERIAL_NUMBER)
          ->Value());

  ld_cfg.set_hw_part_number(
      drv_->GetSensor()
          ->GetProperties()
          ->GetTextProperty(LeddarCore::LdPropertyIds::ID_PART_NUMBER)
          ->Value());

  ld_cfg.set_sw_part_number(
      drv_->GetSensor()
          ->GetProperties()
          ->GetTextProperty(LeddarCore::LdPropertyIds::ID_SOFTWARE_PART_NUMBER)
          ->Value());

  ld_cfg.set_mac_address(
      drv_->GetSensor()
          ->GetProperties()
          ->GetBufferProperty(LeddarCore::LdPropertyIds::ID_MAC_ADDRESS)
          ->GetStringValue());

  running_ = true;
  device_thread_ = std::shared_ptr<std::thread>(new std::thread(
      std::bind(&LeddartechDriverComponent::device_poll, this)));
  device_thread_->detach();

  return true;
}

void LeddartechDriverComponent::device_poll() {
  while (!apollo::cyber::IsShutdown()) {
    std::shared_ptr<LeddartechScan> scan = std::make_shared<LeddartechScan>();
    bool ret = drv_->Poll(scan);

    if (ret) {
      common::util::FillHeader("leddartech", scan.get());
      writer_->Write(scan);
    } else {
      AWARN << "device poll failed";
    }
  }

  AERROR << "LeddartechDriver thread exit";

  running_ = false;
}

}  // namespace leddartech
}  // namespace drivers
}  // namespace apollo
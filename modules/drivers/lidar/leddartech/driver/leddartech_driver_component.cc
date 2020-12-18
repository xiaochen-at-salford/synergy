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

LeddartechDriverComponent::~LeddartechDriverComponent(){
  if (device_thread_->joinable()){
    device_thread_->join();
    drv_->GetSensor()->Disconnect();
  }
}
bool LeddartechDriverComponent::Init() {
  Config leddartech_config;
  if (!GetProtoConfig(&leddartech_config)) {
    return false;
  }

  writer_ = node_->CreateWriter<LeddartechScan>(leddartech_config.scan_channel());
  drv_ = LeddartechDriverFactory::CreateDriver(leddartech_config);

  drv_->Init();

  running_ = true;
  device_thread_ = std::shared_ptr<std::thread> (
      new std::thread(std::bind(&LeddartechDriverComponent::device_poll, this)));
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
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
  AINFO << "LeddarTech driver component init" << std::endl;
  Config leddartech_config;
  if (!GetProtoConfig(&leddartech_config)) {
    std::cout << "cannot open file" << std::endl;
    return false;
  }

  writer_ = node_->CreateWriter<LeddartechScan>("1");
  LeddartechDriver *driver =
      LeddartechDriverFactory::CreateDriver(leddartech_config);

  if (driver == nullptr) {
    std::cout << "cannot open driver" << std::endl;
    return false;
  }

  std::cout << "driver opened" << std::endl;
  drv_.reset(driver);
  drv_->Init();
  drv_->GetSensor()->Connect();
  drv_->GetSensor()->GetConstants();
  drv_->GetSensor()->GetConfig();
  drv_->GetSensor()->SetDataMask(LeddarDevice::LdSensor::DM_ALL);
  running_ = true;
  device_thread_ = std::shared_ptr<std::thread> (
      new std::thread(std::bind(&LeddartechDriverComponent::device_poll, this)));
  device_thread_->detach();

  return true;
}

void LeddartechDriverComponent::device_poll() {
  std::cout << "Device poll" << std::endl;
  while (!apollo::cyber::IsShutdown()) {
    std::shared_ptr<LeddartechScan> scan = std::make_shared<LeddartechScan>();
    bool ret = drv_->Poll(scan);
    std::cout << "One:" << scan->DebugString() << std::endl;
    std::cout << "after one" << std::endl;
    if (ret) {
      std::cout << "before fill hrd" << std::endl;
      common::util::FillHeader("leddartech", scan.get());
      std::cout << "FillHeader" << std::endl;
      //std::cout << "values:" << writer_->Write(scan) << std::endl;
      std::cout << "after writer" << std::endl;
    } else {
      AWARN << "device poll failed";
      std::cout << "error writing" << std::endl;
    }
  }

  AERROR << "LeddartechDriver thread exit";
  std::cout << " error thread exit" << std::endl;
  running_ = false;
}

}  // namespace leddartech
}  // namespace drivers
}  // namespace apollo
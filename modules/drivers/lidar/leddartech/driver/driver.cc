#include "modules/drivers/lidar/leddartech/driver/driver.h"

#include <memory>
#include <string>
#include <thread>

#include "modules/drivers/lidar/leddartech/proto/config.pb.h"
#include "modules/drivers/lidar/leddartech/proto/leddartech.pb.h"

#include "cyber/cyber.h"

namespace apollo {
namespace drivers {
namespace leddartech {

LeddartechDriver::~LeddartechDriver() {}

void LeddartechDriver::Init() {}

bool LeddartechDriver::Poll(const std::shared_ptr<LeddartechScan>& scan) {
  if (GetSensor()->GetData()) {
    LeddarConnection::LdResultEchoes* lResultEchoes =
        GetSensor()->GetResultEchoes();

    uint32_t lDistanceScale = lResultEchoes->GetDistanceScale();
    uint32_t lAmplitudeScale = lResultEchoes->GetAmplitudeScale();

    lResultEchoes->Lock(LeddarConnection::B_GET);
    std::vector<LeddarConnection::LdEcho>& lEchoes =
        *(lResultEchoes->GetEchoes());

    for (uint32_t i = 0; i < lResultEchoes->GetEchoCount(); ++i) {
      LeddartechPacket* packet = scan->add_detections();
      packet->set_channel(lEchoes[i].mChannelIndex);
      packet->set_distance(lEchoes[i].mDistance);
      packet->set_distance_scale(lDistanceScale);
      packet->set_amplitude(lEchoes[i].mAmplitude);
      packet->set_amplitude_scale(lAmplitudeScale);
      packet->set_timestamp(lEchoes[i].mTimestamp);
    }

    lResultEchoes->UnLock(LeddarConnection::B_GET);
  }

  return true;
}

void LeddartechDriver::SetEthernetConnectionInfo(std::string ip,
                                                 uint32_t port) {
  lConnectionInfo =
      std::make_shared<LeddarConnection::LdConnectionInfoEthernet>(
          ip, port, "",
          LeddarConnection::LdConnectionInfo::CT_ETHERNET_LEDDARTECH);
}

void LeddartechDriver::SetEthernetConnection() {
  lConnectionEthernet =
      std::make_shared<LeddarConnection::LdEthernet>(lConnectionInfo.get());
}

void LeddartechDriver::SetEthernetProtocol() {
  lProtocol = std::make_shared<LeddarConnection::LdProtocolLeddartechEthernet>(
      lConnectionInfo.get(), lConnectionEthernet.get());
}

void LeddartechDriver::SetPixelSensor() {
  lSensor = std::make_shared<LeddarDevice::LdSensorPixell>(lProtocol.get());
}

std::shared_ptr<LeddarDevice::LdSensor> LeddartechDriver::GetSensor() {
  return lSensor;
}

LeddartechDriver* LeddartechDriverFactory::CreateDriver(const Config& config) {
  auto new_config = config;
  LeddartechDriver* driver = nullptr;

  std::cout << "new driver" << std::endl;

  driver = new LeddartechDriver(config);
  driver->SetEthernetConnectionInfo(config.ip(), config.port());
  driver->SetEthernetConnection();
  driver->SetEthernetProtocol();
  driver->SetPixelSensor();

  return driver;
}

}  // namespace leddartech
}  // namespace drivers
}  // namespace apollo
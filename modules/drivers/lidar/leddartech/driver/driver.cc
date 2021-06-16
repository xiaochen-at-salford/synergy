#include "modules/drivers/lidar/leddartech/driver/driver.h"

#include <memory>
#include <string>
#include <thread>

#include "LtMathUtils.h"

#include "modules/drivers/lidar/leddartech/proto/config.pb.h"
#include "modules/drivers/lidar/leddartech/proto/leddartech.pb.h"

#include "cyber/cyber.h"
#include "cyber/time/clock.h"

namespace apollo {
namespace drivers {
namespace leddartech {

LeddartechDriver::~LeddartechDriver() { keep_alive_thread_->Stop(); }

void LeddartechDriver::Init() {
  GetSensor()->Connect();
  GetSensor()->GetConstants();
  GetSensor()->GetConfig();
  GetSensor()->SetDataMask(LeddarDevice::LdSensor::DM_ALL);
  keep_alive_thread_.reset(new cyber::Timer(
      10000, [this]() { this->KeepAlive(); }, false));
  keep_alive_thread_->Start();
}

void LeddartechDriver::KeepAlive() { GetSensor()->GetStatus(); }

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
      uint16_t channelIndex = lEchoes[i].mChannelIndex;
      try {
        LeddarUtils::LtMathUtils::LtPointXYZ xyz =
            lEchoes[i].ToXYZ(177.5, 16.0, 96, 8, lDistanceScale);
        packet->set_channel(channelIndex);
        packet->set_distance(lEchoes[i].mDistance);
        packet->set_distance_scale(lDistanceScale);
        packet->set_amplitude(lEchoes[i].mAmplitude);
        packet->set_amplitude_scale(lAmplitudeScale);
        packet->set_timestamp(lEchoes[i].mTimestamp);
        packet->set_hindex(channelIndex % 96);
        packet->set_vindex(channelIndex / 96);
        packet->set_x(xyz.x);
        packet->set_y(xyz.y);
        packet->set_z(xyz.z);
        packet->set_flag(lEchoes[i].mFlag);
      } catch (std::out_of_range) {
        std::cout << "LeddarUtils: error" << std::endl;
        continue;
      }
    }

    lResultEchoes->UnLock(LeddarConnection::B_GET);
  }

  return true;
}

bool LeddartechDriver::Poll(LeddartechScan* scan) 
{
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
      uint16_t channelIndex = lEchoes[i].mChannelIndex;
      try {
        LeddarUtils::LtMathUtils::LtPointXYZ xyz =
            lEchoes[i].ToXYZ(177.5, 16.0, 96, 8, lDistanceScale);
        packet->set_channel(channelIndex);
        packet->set_distance(lEchoes[i].mDistance);
        packet->set_distance_scale(lDistanceScale);
        packet->set_amplitude(lEchoes[i].mAmplitude);
        packet->set_amplitude_scale(lAmplitudeScale);
        packet->set_timestamp(lEchoes[i].mTimestamp);
        packet->set_hindex(channelIndex % 96);
        packet->set_vindex(channelIndex / 96);
        packet->set_x(xyz.x);
        packet->set_y(xyz.y);
        packet->set_z(xyz.z);
        packet->set_flag(lEchoes[i].mFlag);
      } catch (std::out_of_range) {
        std::cout << "LeddarUtils: error" << std::endl;
        continue;
      }
    }

    lResultEchoes->UnLock(LeddarConnection::B_GET);
  }

  return true;
}

void LeddartechDriver::SetEthernetConnectionInfo(std::string ip,
                                                 uint32_t port) {
  lConnectionInfo =
      std::make_unique<LeddarConnection::LdConnectionInfoEthernet>(
          ip, port, "",
          LeddarConnection::LdConnectionInfo::CT_ETHERNET_LEDDARTECH);
}

void LeddartechDriver::SetEthernetConnection() {
  lConnectionEthernet =
      std::make_unique<LeddarConnection::LdEthernet>(lConnectionInfo.get());
}

void LeddartechDriver::SetEthernetProtocol() {
  lProtocol = std::make_unique<LeddarConnection::LdProtocolLeddartechEthernet>(
      lConnectionInfo.get(), lConnectionEthernet.get());
}

void LeddartechDriver::SetPixelSensor() {
  lSensor = std::make_shared<LeddarDevice::LdSensorPixell>(lProtocol.get());
}

std::shared_ptr<LeddarDevice::LdSensorPixell> LeddartechDriver::GetSensor() {
  return lSensor;
}

std::shared_ptr<LeddartechDriver> LeddartechDriverFactory::CreateDriver(
    const Config& config) {
  auto new_config = config;
  std::shared_ptr<LeddartechDriver> driver =
      std::make_shared<LeddartechDriver>(config);

  driver->SetEthernetConnectionInfo(config.ip(), config.port());
  driver->SetEthernetConnection();
  driver->SetEthernetProtocol();
  driver->SetPixelSensor();

  return driver;
}

}  // namespace leddartech
}  // namespace drivers
}  // namespace apollo
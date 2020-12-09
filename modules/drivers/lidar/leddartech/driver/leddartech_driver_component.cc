#include "modules/drivers/lidar/leddartech/driver/leddartech_driver_component.h"
#include "LdConnectionFactory.h"
#include "LdDeviceFactory.h"
#include "LdPropertyIds.h"
#include "LdResultEchoes.h"
#include "LdLjrRecorder.h"
#include "LdRecordPlayer.h"

//Ethernet
#include "LdConnectionInfoEthernet.h"
#include "LdEthernet.h"
#include "LdSensorPixell.h"

//Utils
#include "LtExceptions.h"
#include "LtKeyboardUtils.h"
#include "LtStringUtils.h"
#include "LtTimeUtils.h"
#include "comm/LtComLeddarTechPublic.h"
#include  "cyber/cyber.h"

namespace apollo {
namespace drivers {
namespace leddartech {

LeddartechDriverComponent::LeddartechDriverComponent() {
  AINFO << "Leddartech component constructor";
}

LeddartechDriverComponent::~LeddartechDriverComponent() {
  AINFO << "Leddartech component destuctor";
}

bool LeddartechDriverComponent::Init() {
  auto *lConnectionInfo = new LeddarConnection::LdConnectionInfoEthernet( "192.168.0.2", 48630, "", LeddarConnection::LdConnectionInfo::CT_ETHERNET_LEDDARTECH );
  auto *lConnectionEthernet =  new LeddarConnection::LdEthernet( lConnectionInfo );
  auto *lProtocol = new LeddarConnection::LdProtocolLeddartechEthernet( lConnectionInfo, lConnectionEthernet );
  LeddarDevice::LdSensor *lSensor = new LeddarDevice::LdSensorPixell( lProtocol );

  AINFO << "Leddartech driver" << std::endl;
  // Connect the device
  lSensor->Connect();

  if (lSensor == nullptr) {
    AINFO << "Error connecting to the sensor";
  }
  else if (lSensor != nullptr) {
    lSensor->GetConstants();
    lSensor->GetConfig();
    lSensor->SetDataMask( LeddarDevice::LdSensor::DM_ALL );
    uint32_t count = 0;
    std::cout <<"Sensor found" << std::endl;
    while(count < 10) {
      bool newData = lSensor->GetData();
      if (newData) {
        std::cout << "newData" << std::endl;
        LeddarConnection::LdResultEchoes *lResultEchoes = lSensor->GetResultEchoes();
        uint32_t lDistanceScale = lResultEchoes->GetDistanceScale();
        uint32_t lAmplitudeScale = lResultEchoes->GetAmplitudeScale();

        lResultEchoes->Lock(LeddarConnection::B_GET);
        std::cout << "Pixell" << std::endl;
        std::cout << "Channel\tDistance\tAmplitude" << std::endl;
        std::vector<LeddarConnection::LdEcho> &lEchoes = *( lResultEchoes->GetEchoes() );

        for( uint32_t i = 0; i < lResultEchoes->GetEchoCount(); ++i )
        {
          std::cout << lEchoes[i].mChannelIndex;
          std::cout << "\t" << std::setprecision( 3 ) << ( ( float )lEchoes[i].mDistance / ( float )lDistanceScale );
          std::cout << "\t\t" << std::setprecision( 3 ) << ( ( float )lEchoes[i].mAmplitude / ( float )lAmplitudeScale );

          if( lEchoes[i].mTimestamp != 0 ) {
            std::cout << "\t@" << lEchoes[i].mTimestamp << std::endl;
          }
          std::cout << std::endl;
        }

        lResultEchoes->UnLock( LeddarConnection::B_GET );
        count++;
      } else {
        std::cout << "no new data" << std::endl;
      }
  }
  lSensor->Disconnect();
  delete lSensor;
  }
  return true;
}


}
}
}
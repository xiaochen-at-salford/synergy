#pragma once

#include <memory>
#include <string>

#include "modules/drivers/lidar/leddartech/proto/config.pb.h"
#include "modules/drivers/lidar/leddartech/proto/leddartech.pb.h"

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

namespace apollo {
namespace drivers {
namespace leddartech {

class LeddartechDriver {
 public:
    explicit LeddartechDriver(const Config &config) : config_(config){}
    virtual ~LeddartechDriver();
    std::shared_ptr<LeddarDevice::LdSensor> GetSensor();
    virtual bool Poll(const std::shared_ptr<LeddartechScan> &scan);

    virtual void Init();
    void SetEthernetConnectionInfo(std::string ip, uint32_t port);
    void SetEthernetConnection();
    void SetEthernetProtocol();
    void SetPixelSensor();

 protected:
    Config config_;
    std::string topic_;
    std::shared_ptr<LeddarConnection::LdConnectionInfoEthernet> lConnectionInfo;
    std::shared_ptr<LeddarConnection::LdEthernet> lConnectionEthernet;
    std::shared_ptr<LeddarConnection::LdProtocolLeddartechEthernet> lProtocol;
    std::shared_ptr<LeddarDevice::LdSensorPixell> lSensor;
};

class LeddartechDriverFactory {
    public:
        static LeddartechDriver *CreateDriver(const Config &config);
};

} //leddartech
} //drivers
} //apollo
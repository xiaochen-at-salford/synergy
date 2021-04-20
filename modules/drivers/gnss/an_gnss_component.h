#pragma once

#include <memory>
#include <string>
#include <vector>

#include "cyber/cyber.h"
#include "modules/common/monitor_log/monitor_log_buffer.h"

// #include "modules/drivers/gnss/stream/raw_stream.h"
#include "modules/drivers/gnss/stream/an_raw_stream.h"

namespace apollo {
namespace drivers {
namespace gnss {

using apollo::cyber::Component;
using apollo::cyber::Reader;
using apollo::cyber::Writer;
using apollo::drivers::gnss::RawData;

class AnGnssDriverComponent : public Component<> {
 public:
  AnGnssDriverComponent();
  bool Init() override;

 private:
  std::unique_ptr<AnRawStream> raw_stream_ = nullptr; // The AnRawStream doesnot deal with the RTK thing
  apollo::common::monitor::MonitorLogBuffer monitor_logger_buffer_;
};

CYBER_REGISTER_COMPONENT(AnGnssDriverComponent)

}  // namespace gnss
}  // namespace drivers
}  // namespace apollo

#include "modules/drivers/gnss/an_gnss_component.h"

namespace apollo {
namespace drivers {
namespace gnss {

using apollo::cyber::proto::RoleAttributes;

AnGnssDriverComponent::AnGnssDriverComponent()
  : monitor_logger_buffer_(apollo::common::monitor::MonitorMessageItem::GNSS) 
{}

bool AnGnssDriverComponent::Init() 
{
  config::Config gnss_config;

  //TODO:check later
  if (!apollo::cyber::common::GetProtoFromFile(config_file_path_, &gnss_config)) 
  {
    monitor_logger_buffer_.ERROR("Unable to load gnss conf file: " + config_file_path_);
    return false;
  }
  AINFO << "Gnss config: " << gnss_config.DebugString();

  raw_stream_.reset(new AnRawStream(gnss_config, node_));

  if (!raw_stream_->Init()) 
  {
    monitor_logger_buffer_.ERROR("Init gnss stream failed");
    AERROR << "Init gnss stream failed";
    return false;
  }
  raw_stream_->Start();
  monitor_logger_buffer_.INFO("gnss is started.");
  return true;
}

}  // namespace gnss
}  // namespace drivers
}  // namespace apollo

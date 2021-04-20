#include "modules/drivers/gnss/an_relay_component.h"

namespace apollo {
namespace drivers {
namespace gnss {

bool AnRelayComponent::Init()
{
  writer_ = node_->CreateWriter<CorrectedImu>("/apollo/sensor/gnss/corrected_imu");
  return true; 
}

bool AnRelayComponent::Proc(const std::shared_ptr<AnGnss>& an_gnss)
{
  auto corrected_imu = CorrectedImu();
  corrected_imu.mutable_imu()->mutable_linear_velocity()->set_x(111);
  writer_->Write(corrected_imu);
  return true;
}

} // namespace gnss
} // namespace drivers
} // namespace apollo
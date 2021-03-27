#pragma once

#include <memory>

#include "modules/canbus/proto/vehicle_parameter.pb.h"
#include "modules/canbus/vehicle/abstract_vehicle_factory.h"
#include "modules/canbus/vehicle/vehicle_controller.h"
#include "modules/drivers/canbus/can_comm/message_manager.h"

namespace apollo {
namespace canbus {

class NiroVehicleFactory : public AbstractVehicleFactory {
 public:
  virtual ~NiroVehicleFactory() = default;

  std::unique_ptr<VehicleController> CreateVehicleController() override;

  std::unique_ptr<MessageManager<::apollo::canbus::ChassisDetail>>
  CreateMessageManager() override;
};

} // namespace canbus
} // namespace apollo
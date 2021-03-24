#pragma once

#include <memory>

#include "modules/canbus/proto/vehicle_parameter.pb.h"
#include "modules/canbus/vehicle/abstract_vehicle_factory.h"
#include "modules/canbus/vehicle/vehicle_controller.h"
#include "modules/drivers/canbus/can_comm/message_manager.h"

/**
 * @namespace apollo::canbus
 * @brief apollo::canbus
 */
namespace apollo {
namespace canbus {

/**
 * @class DevkitVehicleFactory
 *
 * @brief this class is inherited from AbstractVehicleFactory. It can be used to
 * create controller and message manager for devkit vehicle.
 */
class OsccVehicleFactory : public AbstractVehicleFactory {
 public:
  /**
   * @brief destructor
   */
  virtual ~OsccVehicleFactory() = default;

  /**
   * @brief create devkit vehicle controller
   * @returns a unique_ptr that points to the created controller
   */
  std::unique_ptr<VehicleController> CreateVehicleController() override;

  /**
   * @brief create devkit message manager
   * @returns a unique_ptr that points to the created message manager
   */
  std::unique_ptr<MessageManager<::apollo::canbus::ChassisDetail>>
  CreateMessageManager() override;
};

}  // namespace canbus
}  // namespace apollo
#include "modules/canbus/vehicle/niro/niro_vehicle_factory.h"

#include "cyber/common/log.h"
#include "modules/common/util/util.h"
#include "modules/canbus/vehicle/niro/niro_controller.h"
#include "modules/canbus/vehicle/niro/niro_message_manager.h"

namespace apollo {
namespace canbus {

using ::apollo::canbus::ChassisDetail;

std::unique_ptr<VehicleController>
NiroVehicleFactory::CreateVehicleController() 
{ return std::unique_ptr<VehicleController>(new niro::NiroController()); }

std::unique_ptr<MessageManager<ChassisDetail>>
NiroVehicleFactory::CreateMessageManager() 
{
  return std::unique_ptr<MessageManager<ChassisDetail>>(
      new niro::NiroMessageManager() );
}

}  // namespace canbus
}  // namespace apollo
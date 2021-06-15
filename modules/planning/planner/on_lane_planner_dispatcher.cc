#include "modules/planning/planner/on_lane_planner_dispatcher.h"

#include <memory>

#include "modules/planning/proto/planning_config.pb.h"

namespace apollo {
namespace planning {

std::unique_ptr<Planner> OnLanePlannerDispatcher::DispatchPlanner(
    const PlanningConfig &planning_config,
    const std::shared_ptr<DependencyInjector> &injector ) 
{
  return planner_factory_.CreateObject(
      planning_config.standard_planning_config().planner_type(0), injector );
}

}  // namespace planning
}  // namespace apollo

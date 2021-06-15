#include "modules/planning/planner/planner_dispatcher.h"

#include <memory>

#include "modules/planning/planner/lattice/lattice_planner.h"
#include "modules/planning/planner/navi/navi_planner.h"
#include "modules/planning/planner/public_road/public_road_planner.h"
#include "modules/planning/planner/rtk/rtk_replay_planner.h"
#include "modules/planning/proto/planning_config.pb.h"

namespace apollo {
namespace planning {

void PlannerDispatcher::RegisterPlanners() 
{
  planner_factory_.Register(
      PlannerType::RTK,
      []
      (const std::shared_ptr<DependencyInjector>& injector) -> Planner* 
      { return new RTKReplayPlanner(injector); } );

  planner_factory_.Register(
      PlannerType::PUBLIC_ROAD,
      []
      (const std::shared_ptr<DependencyInjector>& injector) -> Planner* 
      { return new PublicRoadPlanner(injector); } );

  planner_factory_.Register(
      PlannerType::LATTICE,
      []
      (const std::shared_ptr<DependencyInjector>& injector) -> Planner* 
      { return new LatticePlanner(injector); } );

  planner_factory_.Register(
      PlannerType::NAVI,
      []
      (const std::shared_ptr<DependencyInjector>& injector) -> Planner* 
      { return new NaviPlanner(injector); } );
}

}  // namespace planning
}  // namespace apollo

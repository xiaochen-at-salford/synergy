#pragma once

#include <memory>

#include "modules/common/util/factory.h"
#include "modules/planning/planner/planner_dispatcher.h"

namespace apollo {
namespace planning {

class OnLanePlannerDispatcher final : public PlannerDispatcher {
 public:
  OnLanePlannerDispatcher() = default;

  virtual ~OnLanePlannerDispatcher() = default;

  std::unique_ptr<Planner> DispatchPlanner(
      const PlanningConfig &planning_config,
      const std::shared_ptr<DependencyInjector> &injector ) override;
};

}  // namespace planning
}  // namespace apollo

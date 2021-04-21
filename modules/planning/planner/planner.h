#pragma once

#include <memory>
#include <string>

#include "modules/common/proto/pnc_point.pb.h"
#include "modules/common/status/status.h"
#include "modules/planning/common/frame.h"
#include "modules/planning/proto/planning_config.pb.h"
#include "modules/planning/scenarios/scenario.h"
#include "modules/planning/scenarios/scenario_manager.h"

namespace apollo {
namespace planning {

/**
 * @class Planner
 * @brief Planner is a base class for specific planners.
 *        It contains a pure virtual function Plan which must be implemented in
 *        derived class.
 */
class Planner {
 public:
  Planner() = delete;

  explicit Planner(const std::shared_ptr<DependencyInjector>& injector)
      : scenario_manager_(injector) 
      {}

  virtual ~Planner() = default;

  virtual std::string Name() = 0;

  virtual apollo::common::Status Init(const PlanningConfig& config) = 0;

  /**
   * @brief Compute trajectories for execution.
   * @param planning_init_point The trajectory point where planning starts.
   * @param frame Current planning frame.
   * @return OK if planning succeeds; error otherwise.
   */
  virtual apollo::common::Status Plan(
      const common::TrajectoryPoint &planning_init_point, 
      Frame *frame,
      ADCTrajectory *ptr_computed_trajectory ) = 0;

  virtual void Stop() = 0;

 protected:
  PlanningConfig config_;
  scenario::ScenarioManager scenario_manager_;
  scenario::Scenario* scenario_ = nullptr;
};

class PlannerWithReferenceLine : public Planner {
 public:
  PlannerWithReferenceLine() = delete;

  explicit PlannerWithReferenceLine(
      const std::shared_ptr<DependencyInjector> &injector)
      : Planner(injector) 
      {}

  virtual ~PlannerWithReferenceLine() = default;

  /**
   * @brief Compute a trajectory for execution.
   * @param planning_init_point The trajectory point where planning starts.
   * @param frame Current planning frame.
   * @param reference_line_info The computed reference line.
   * @return OK if planning succeeds; error otherwise.
   */
  virtual apollo::common::Status PlanOnReferenceLine(
      const common::TrajectoryPoint &planning_init_point, 
      Frame *frame,
      ReferenceLineInfo *reference_line_info ) 
  {
    CHECK_NOTNULL(frame);
    return apollo::common::Status::OK();
  }
};

}  // namespace planning
}  // namespace apollo

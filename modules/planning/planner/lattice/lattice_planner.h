#pragma once

#include <memory>
#include <string>

#include "modules/common/status/status.h"
#include "modules/planning/common/frame.h"
#include "modules/planning/common/reference_line_info.h"
#include "modules/planning/planner/planner.h"
#include "modules/planning/proto/planning_config.pb.h"

namespace apollo {
namespace planning {

class LatticePlanner : public PlannerWithReferenceLine {
 public:
  LatticePlanner() = delete;

  explicit LatticePlanner(const std::shared_ptr<DependencyInjector>& injector)
      : PlannerWithReferenceLine(injector) 
      {}

  virtual ~LatticePlanner() = default;

  std::string Name() override { return "LATTICE"; }

  common::Status Init(const PlanningConfig& config) 
  override { return common::Status::OK(); }

  void Stop() override {}

  /**
   * @brief Override function Plan in parent class Planner.
   * @param planning_init_point The trajectory point where planning starts.
   * @param frame Current planning frame.
   * @return OK if planning succeeds; error otherwise.
   */
  common::Status Plan(const common::TrajectoryPoint &planning_init_point,
                      Frame *frame,
                      ADCTrajectory* ptr_computed_trajectory ) override;

  /**
   * @brief Override function Plan in parent class Planner.
   * @param planning_init_point The trajectory point where planning starts.
   * @param frame Current planning frame.
   * @param reference_line_info The computed reference line.
   * @return OK if planning succeeds; error otherwise.
   */
  common::Status PlanOnReferenceLine(
      const common::TrajectoryPoint &planning_init_point, 
      Frame *frame,
      ReferenceLineInfo *reference_line_info ) override;
};

}  // namespace planning
}  // namespace apollo

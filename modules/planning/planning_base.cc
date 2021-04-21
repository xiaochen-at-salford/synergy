#include "modules/planning/planning_base.h"

#include "cyber/time/clock.h"
#include "modules/map/hdmap/hdmap_util.h"
#include "modules/planning/common/planning_context.h"
#include "modules/planning/common/planning_gflags.h"
#include "modules/planning/proto/planning_internal.pb.h"
#include "modules/planning/tasks/task_factory.h"

namespace apollo {
namespace planning {

using apollo::common::Status;

PlanningBase::PlanningBase(const std::shared_ptr<DependencyInjector> &injector)
  : injector_(injector) {}

PlanningBase::~PlanningBase() {}

Status PlanningBase::Init(const PlanningConfig& config) 
{
  injector_->planning_context()->Init();
  TaskFactory::Init(config, injector_);
  return Status::OK();
}

void PlanningBase::FillPlanningPb(const double timestamp, ADCTrajectory* const trajectory_pb) 
{
  trajectory_pb->mutable_header()->set_timestamp_sec(timestamp);
  if (local_view_.prediction_obstacles->has_header()) 
  {
    trajectory_pb->mutable_header()->set_lidar_timestamp(
        local_view_.prediction_obstacles->header().lidar_timestamp() );
    trajectory_pb->mutable_header()->set_camera_timestamp(
        local_view_.prediction_obstacles->header().camera_timestamp() );
    trajectory_pb->mutable_header()->set_radar_timestamp(
        local_view_.prediction_obstacles->header().radar_timestamp() );
  }

  trajectory_pb->mutable_routing_header()->CopyFrom(
      local_view_.routing->header() );
}

}  // namespace planning
}  // namespace apollo

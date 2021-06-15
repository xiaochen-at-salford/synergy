#pragma once

#include <memory>

#include "modules/routing/routing.h"

namespace apollo {
namespace routing {

class RoutingComponent final
    : public ::apollo::cyber::Component<RoutingRequest> {
 public:
  RoutingComponent() = default;

  ~RoutingComponent() = default;

 public:
  bool Init() override;

  bool Proc(const std::shared_ptr<RoutingRequest>& request) override;

 private:
  std::shared_ptr<::apollo::cyber::Writer<RoutingResponse>> response_writer_ = nullptr;
  std::shared_ptr<::apollo::cyber::Writer<RoutingResponse>> response_history_writer_ = nullptr;
  Routing routing_;
  std::shared_ptr<RoutingResponse> response_ = nullptr;
  std::unique_ptr<::apollo::cyber::Timer> timer_;
  std::mutex mutex_;
};

CYBER_REGISTER_COMPONENT(RoutingComponent)

}  // namespace routing
}  // namespace apollo

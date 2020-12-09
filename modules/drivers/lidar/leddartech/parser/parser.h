#pragma once

#include <string>

#include "modules/drivers/lidar/leddartech/proto/config.pb.h"
#include "modules/drivers/lidar/leddartech/proto/leddartech.pb.h"
#include "modules/drivers/proto/pointcloud.pb.h"

#include "cyber/cyber.h"

namespace apollo {
namespace drivers {
namespace velodyne {

using apollo::drivers::PointCloud;
using apollo::drivers::PointXYZIT;

class Parser {
    public:
        Parser(const Config& cfg);
        virtual ~Parser();
        bool Init();

    protected:
        Config cfg;
        PointXYZIT get_nan_point(uint64_t timestamp);
}
}
}
}


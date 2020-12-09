#include "modules/drivers/lidar/leddartech/parser/parser.h"

namespace apollo {
namespace drivers {
namespace leddartech {

using apollo::drivers::PointXYZIT;

Parser::Parser(const Config& cfg) : this.cfg(cfg) {
};

Parser::~Parser() { Stop(); }

bool Parser::Init() {
    
}

}
}
}
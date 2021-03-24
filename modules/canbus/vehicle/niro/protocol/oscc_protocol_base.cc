#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

void OsccProtocolBase::set_p_magic(uint8_t *data)
{
  Byte to_set(data+0);
  to_set.set_value(OSCC_MAGIC_BYTE_0, 0, 1);
  to_set.set_value(OSCC_MAGIC_BYTE_1, 1, 1);
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
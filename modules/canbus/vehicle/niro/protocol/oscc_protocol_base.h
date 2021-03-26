#pragma once

#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

class OsccProtocolBase {
 public:
  /**
   * @brief First magic byte used in commands and reports to distinguish CAN
   *        frame as coming from OSCC (and not OBD). 
   */
  static const int8_t OSCC_MAGIC_BYTE_0 = 0x05;

/**
 * @brief Second magic byte used in commands and reports to distinguish CAN
 *        frame as coming from OSCC (and not OBD).
 */
  static const int8_t OSCC_MAGIC_BYTE_1 = 0x05;

  void set_p_magic(uint8_t *data);
}; 

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
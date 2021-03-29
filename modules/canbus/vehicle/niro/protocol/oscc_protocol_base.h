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

 protected:
  void set_use_magic_header(bool use_magic_header) 
  { use_magic_header_ = use_magic_header; }

  bool has_magic_header()
  { return use_magic_header_; }

 private:
  bool use_magic_header_ = false;
}; 

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
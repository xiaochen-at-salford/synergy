#pragma once

#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

//TODO(xiaochen): Why decimal magical number is 1372 from the OSCC dbc file?

class OsccProtocolBase 
  : public ::apollo::drivers::canbus::ProtocolData<::apollo::canbus::ChassisDetail> {
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
  static const int8_t OSCC_MAGIC_BYTE_1 = 0xCC;

  bool is_active() const;

  void activate();

  void deactivate();

  bool is_auto_active() const;
  
  void enable_auto_activation();

  void disable_auto_activation();

  bool use_magic() const;

  void enable_magic_use();

  void disable_magic_use();

  void set_p_magic(uint8_t *data);

 protected:
  // bool is_active_ = false;
  // bool is_auto_active_ = false;
  bool use_magic_ = false;
}; 

}  // namespace niro
}  // namespace canbus
}  // namespace apollo

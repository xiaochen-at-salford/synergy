#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"

namespace apollo {
namespace canbus {
namespace niro {

// using ::apollo::drivers::canbus::Byte;

// bool OsccProtocolBase::is_active()
// const { return is_active_; }

// void OsccProtocolBase::activate()
// { is_active_ = true; }

// void OsccProtocolBase::deactivate()
// { is_active_ = false; }

// bool OsccProtocolBase::is_auto_active() 
// const { return is_auto_active_; }

// void OsccProtocolBase::enable_auto_activation() 
// { is_auto_active_ = true; }

// void OsccProtocolBase::disable_auto_activation() 
// { is_auto_active_ = false; }

bool OsccProtocolBase::use_magic()
const { return use_magic_; }

void OsccProtocolBase::enable_magic_use()
{ use_magic_ = true; }

void OsccProtocolBase::disable_magic_use()
{ use_magic_ = false; }

void OsccProtocolBase::set_p_magic(uint8_t *data)
{
  data[0] = OSCC_MAGIC_BYTE_0;
  data[1] = OSCC_MAGIC_BYTE_1;
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
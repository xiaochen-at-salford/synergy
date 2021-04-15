#include "modules/canbus/vehicle/niro/protocol/0d175_0xaf_fault_report.h"

#include "modules/drivers/canbus/common/byte.h"

namespace apollo {
namespace canbus {
namespace niro {

using ::apollo::drivers::canbus::Byte;

FaultReport_0xAF::FaultReport_0xAF() {};

void FaultReport_0xAF::Parse(const std::uint8_t *bytes, int32_t length, ChassisDetail *chassis) 
const {
  chassis->mutable_niro()->mutable_fault_report_0xaf(
      )->set_fault_origin_id(fault_origin_id(bytes, length)) ;

  chassis->mutable_niro()->mutable_fault_report_0xaf(
      )->set_fault_dtcs(fault_dtcs(bytes, length)) ;


}

FaultOriginIdEnum FaultReport_0xAF::fault_origin_id(const std::uint8_t *bytes, int32_t length) 
const {
  FaultOriginIdEnum x = FaultOriginIdEnum::Fault_Report_0xAF_FaultOriginId_Enum_BRAKE_CONTROL_CHANNEL;
  return x;
}

FaultDtcsEnum FaultReport_0xAF::fault_dtcs(const std::uint8_t *bytes, int32_t length) 
const {
  FaultDtcsEnum x = FaultDtcsEnum::Fault_Report_0xAF_FaultDtcs_Enum_NO_COMMUNICATION;
  return x;
}

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
#pragma once

#include "modules/canbus/vehicle/niro/protocol/oscc_protocol_base.h"
#include "modules/canbus/proto/chassis_detail.pb.h"
#include "modules/drivers/canbus/can_comm/protocol_data.h"

namespace apollo {
namespace canbus {
namespace niro {

/* From OSCC SDK:
 * typedef struct {
 *   uint8_t reserved_0[4]; // Reserved
 *   int16_t master_cylinder_pressure; // 1/10th of a bar per bit 
 *   uint8_t reserved_1[2]; // Reserved.
 * } kia_soul_obd_brake_pressure_data_s;
 */ 

/* From PolySync Chassi KIA Niro DBC:
 *
 * BO_ 544 BRAKE_PRESSURE: 8 VEHICLE
 *  SG_ BRAKE_PRESSURE : 24|12@1+ (0.025,0) [0|0] "bar" VEHICLE
 * 
 * CM_ BO_ 544 "Vehicle Brake Pressure";
 */

class BrakePressure_0x220 
  : public OsccProtocolBase,
    public ::apollo::drivers::canbus::ProtocolData<::apollo::canbus::ChassisDetail> {

 public:
  static const int32_t ID;

  BrakePressure_0x220();

  void Parse(const std::uint8_t *bytes, int32_t length, ChassisDetail *chassis) const override;

 private:
  double brake_pressure(const std::uint8_t *bytes, int32_t length) const;
};

}  // namespace niro
}  // namespace canbus
}  // namespace apollo
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <math.h>

#include "cyber/cyber.h"

// #include "modules/drivers/gnss/parser/novatel_messages.h" // Disabled for syn
#include "modules/drivers/gnss/parser/parser.h"
// #include "modules/drivers/gnss/parser/rtcm_decode.h" // Disabled for syn
#include "modules/drivers/gnss/proto/gnss.pb.h"
#include "modules/drivers/gnss/proto/gnss_best_pose.pb.h"
#include "modules/drivers/gnss/proto/gnss_raw_observation.pb.h"
#include "modules/drivers/gnss/proto/heading.pb.h"
#include "modules/drivers/gnss/proto/imu.pb.h"
#include "modules/drivers/gnss/proto/ins.pb.h"
#include "modules/drivers/gnss/util/time_conversion.h"
#include "modules/drivers/gnss/proto/an_gnss.pb.h"

#include "modules/drivers/gnss/an/an_packet_protocol.h"
#include "modules/drivers/gnss/an/spatial_packets.h"

namespace apollo {
namespace drivers {
namespace gnss {

namespace {
#define RADIANS_TO_DEGREES (180.0/M_PI)
constexpr float FLOAT_NAN = std::numeric_limits<float>::quiet_NaN();
}


class AnParser : public Parser {
 public:
  AnParser();
  explicit AnParser(const config::Config &config);

  virtual MessageType GetMessage(MessagePtr *message_ptr);

 private:
  ::apollo::drivers::gnss::AnGnss an_system_state_;

  // For decoding
  an_decoder_t an_decoder_;
	an_packet_t *an_packet_;
	system_state_packet_t system_state_packet_;

  /**
   * @brief Assign the decoded system_state_packet to its Apollo msg
   */
  void process_system_state_msg();
};

Parser *Parser::CreateAn(const config::Config& config) 
{ return new AnParser(config); }

namespace {
  constexpr size_t BUFFER_SIZE = 2056;
}

AnParser::AnParser() 
{
  // buffer_.reserve(BUFFER_SIZE);
  an_decoder_initialise(&an_decoder_);
  an_system_state_.mutable_imu()->mutable_orientation_covariance()->Resize(9, FLOAT_NAN);
  an_system_state_.mutable_imu()->mutable_angular_velocity_covariance()->Resize(9, FLOAT_NAN);
  an_system_state_.mutable_imu()->mutable_linear_acceleration_covariance()->Resize(9, FLOAT_NAN);

  // ins_.mutable_position_covariance()->Resize(9, FLOAT_NAN);
  // ins_.mutable_euler_angles_covariance()->Resize(9, FLOAT_NAN);
  // ins_.mutable_linear_velocity_covariance()->Resize(9, FLOAT_NAN);

  // if (1 != init_raw(&raw_)) 
  // { AFATAL << "memory allocation error for observation data structure."; }
}

AnParser::AnParser(const config::Config &config) 
{
  // buffer_.reserve(BUFFER_SIZE);
  an_decoder_initialise(&an_decoder_);
  an_system_state_.mutable_imu()->mutable_orientation_covariance()->Resize(9, FLOAT_NAN);
  an_system_state_.mutable_imu()->mutable_angular_velocity_covariance()->Resize(9, FLOAT_NAN);
  an_system_state_.mutable_imu()->mutable_linear_acceleration_covariance()->Resize(9, FLOAT_NAN);

  // ins_.mutable_position_covariance()->Resize(9, FLOAT_NAN);
  // ins_.mutable_euler_angles_covariance()->Resize(9, FLOAT_NAN);
  // ins_.mutable_linear_velocity_covariance()->Resize(9, FLOAT_NAN);

  // if (config.has_imu_type()) 
  // {
  //   imu_type_ = config.imu_type();
  // }

  // if (1 != init_raw(&raw_)) 
  // {
  //   AFATAL << "memory allocation error for observation data structure.";
  // }
}


//TODO(xiaochen-at-salford):
Parser::MessageType AnParser::GetMessage(MessagePtr *message_ptr) 
{
  auto message_type = MessageType::NONE; 

  if (data_ == nullptr)
  { return message_type; }

  if (data_size_ > 0)
  {
    std::copy(data_, data_end_, an_decoder_pointer(&an_decoder_));
    an_decoder_increment(&an_decoder_, data_size_);

    while ((an_packet_ = an_packet_decode(&an_decoder_)) != NULL)
    {
      if (an_packet_->id == packet_id_system_state)
      {
        if(decode_system_state_packet(&system_state_packet_, an_packet_) == 0)
        {
          process_system_state_msg();  
          message_type = MessageType::AN_SYSTEM_STATE;
          *message_ptr = &an_system_state_;
        }
      }
    }

    an_packet_free(&an_packet_);
    data_size_ = 0;
  }
    
  return message_type;
}

void AnParser::process_system_state_msg()
{
  /**
   * Time
   */
  an_system_state_.set_unix_time_seconds(system_state_packet_.unix_time_seconds);
  an_system_state_.set_microseconds(system_state_packet_.microseconds);
  double time = static_cast<double>(an_system_state_.unix_time_seconds())
                + static_cast<double>(an_system_state_.microseconds())/1000000.0; 
  an_system_state_.set_measurement_time(time);


  an_system_state_.mutable_nav_sat_fix()->set_latitude(system_state_packet_.latitude*RADIANS_TO_DEGREES);
  an_system_state_.mutable_nav_sat_fix()->set_longitude(system_state_packet_.longitude*RADIANS_TO_DEGREES);
  an_system_state_.mutable_nav_sat_fix()->set_altitude(system_state_packet_.height);


  // auto x = system_state_packet_.latitude*RADIANS_TO_DEGREES;
  // auto y = system_state_packet_.longitude*RADIANS_TO_DEGREES;
  // auto z = system_state_packet_.height;

  // Convert roll, pitch, yaw from radians to quaternion format
  float phi = system_state_packet_.orientation[0] / 2.0f;
  float theta = system_state_packet_.orientation[1] / 2.0f;
  // Add "-", AN and Apollo use oppposite z axes
  float psi = - system_state_packet_.orientation[2] / 2.0f;
  float sin_phi = sinf(phi);
  float cos_phi = cosf(phi);
  float sin_theta = sinf(theta);
  float cos_theta = cosf(theta);
  float sin_psi = sinf(psi);
  float cos_psi = cosf(psi);

  auto&& quat_x = -cos_phi * sin_theta * sin_psi + sin_phi * cos_theta * cos_psi;
  auto&& quat_y = cos_phi * sin_theta * cos_psi + sin_phi * cos_theta * sin_psi;
  auto&& quat_z = cos_phi * cos_theta * sin_psi - sin_phi * sin_theta * cos_psi;
	auto&& quat_w = cos_phi * cos_theta * cos_psi + sin_phi * sin_theta * sin_psi;

  an_system_state_.mutable_imu()->mutable_euler_angles()->set_x(phi);
  an_system_state_.mutable_imu()->mutable_euler_angles()->set_y(theta);
  an_system_state_.mutable_imu()->mutable_euler_angles()->set_z(phi);

  an_system_state_.mutable_imu()->mutable_orientation()->set_qx(quat_x); 
  an_system_state_.mutable_imu()->mutable_orientation()->set_qy(quat_y); 
  an_system_state_.mutable_imu()->mutable_orientation()->set_qz(quat_z); 
  an_system_state_.mutable_imu()->mutable_orientation()->set_qw(quat_w); 

  an_system_state_.mutable_imu()->mutable_angular_velocity()->set_x(system_state_packet_.angular_velocity[0]); 
  an_system_state_.mutable_imu()->mutable_angular_velocity()->set_y(system_state_packet_.angular_velocity[1]); 
  an_system_state_.mutable_imu()->mutable_angular_velocity()->set_z(system_state_packet_.angular_velocity[2]); 

  an_system_state_.mutable_imu()->mutable_linear_acceleration()->set_x(system_state_packet_.body_acceleration[0]); 
  an_system_state_.mutable_imu()->mutable_linear_acceleration()->set_y(system_state_packet_.body_acceleration[1]); 
  an_system_state_.mutable_imu()->mutable_linear_acceleration()->set_z(system_state_packet_.body_acceleration[2]); 

  an_system_state_.mutable_twist()->mutable_linear()->set_x(system_state_packet_.velocity[0]);
  an_system_state_.mutable_twist()->mutable_linear()->set_y(system_state_packet_.velocity[1]);
  an_system_state_.mutable_twist()->mutable_linear()->set_z(system_state_packet_.velocity[2]);

  an_system_state_.mutable_twist()->mutable_angular()->set_x(system_state_packet_.angular_velocity[0]); 
  an_system_state_.mutable_twist()->mutable_angular()->set_y(system_state_packet_.angular_velocity[1]); 
  an_system_state_.mutable_twist()->mutable_angular()->set_z(system_state_packet_.angular_velocity[2]); 
}

} // namespace gnss
} // namespace drivers
} // apollo
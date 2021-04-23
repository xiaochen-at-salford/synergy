#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <thread>

#include "cyber/cyber.h"

#include "modules/canbus/proto/chassis.pb.h"
#include "modules/drivers/gnss/proto/config.pb.h"
#include "modules/drivers/gnss/proto/gnss_status.pb.h"
#include "modules/localization/proto/gps.pb.h"


#include "modules/drivers/gnss/parser/data_parser.h"
// #include "modules/drivers/gnss/parser/rtcm_parser.h"
#include "modules/drivers/gnss/stream/stream.h"

namespace apollo {
namespace drivers {
namespace gnss {

using apollo::localization::Gps;

class AnRawStream {
 public:
  AnRawStream(const config::Config &config,
              const std::shared_ptr<apollo::cyber::Node> &node );
  ~AnRawStream();
  bool Init();

  struct Status {
    bool filter[Stream::NUM_STATUS] = { false };
    Stream::Status status;
  };

  void Start();

 private:
  void DataSpin();
  // void RtkSpin(); // Removed for syn
  bool Connect();
  bool Disconnect();
  // bool Login(); // Disabled for syn
  // bool Logout(); // Disabled for syn
  void StreamStatusCheck();
  // void PublishRtkData(size_t length);
  // void PushGpgga(size_t length);
  void GpsbinSpin();
  void GpsbinCallback(const std::shared_ptr<RawData const>& raw_data);
  // void OnWheelVelocityTimer();

  std::unique_ptr<cyber::Timer> wheel_velocity_timer_ = nullptr;
  std::shared_ptr<apollo::canbus::Chassis> chassis_ptr_ = nullptr;
  static constexpr size_t BUFFER_SIZE = 2048;
  uint8_t buffer_[BUFFER_SIZE] = { 0 };
  // uint8_t buffer_rtk_[BUFFER_SIZE] = { 0 }; // Disabled for syn

  std::shared_ptr<Stream> data_stream_;
  // std::shared_ptr<Stream> command_stream_; // Disabled for syn
  // std::shared_ptr<Stream> in_rtk_stream_; // Disabled for syn
  // std::shared_ptr<Stream> out_rtk_stream_; // Disabled for syn

  std::shared_ptr<Status> data_stream_status_;
  // std::shared_ptr<Status> command_stream_status_; // Disabled for syn
  // std::shared_ptr<Status> in_rtk_stream_status_; // Disabled for syn
  // std::shared_ptr<Status> out_rtk_stream_status_; // Disabled for syn

  // bool rtk_software_solution_ = false; // Disalbed for syn
  bool push_location_ = false;
  bool is_healthy_ = true;
  config::Config config_;

  const std::string raw_data_topic_;
  // const std::string rtcm_data_topic_;

  StreamStatus stream_status_;
  std::unique_ptr<std::thread> data_thread_ptr_;
  // std::unique_ptr<std::thread> rtk_thread_ptr_;
  std::unique_ptr<DataParser> data_parser_ptr_;
  // std::unique_ptr<RtcmParser> rtcm_parser_ptr_;
  std::unique_ptr<std::thread> gpsbin_thread_ptr_;
  std::unique_ptr<std::ofstream> gpsbin_stream_ = nullptr;

  std::shared_ptr<apollo::cyber::Node> node_ = nullptr;
  std::shared_ptr<apollo::cyber::Writer<StreamStatus>> stream_writer_ = nullptr;
  // std::shared_ptr<apollo::cyber::Writer<Gps>> gps_writer_ = nullptr;
  std::shared_ptr<apollo::cyber::Writer<RawData>> raw_writer_ = nullptr;
  // std::shared_ptr<apollo::cyber::Writer<RawData>> rtcm_writer_ = nullptr;
  std::shared_ptr<apollo::cyber::Reader<RawData>> gpsbin_reader_ = nullptr;
  std::shared_ptr<apollo::cyber::Reader<apollo::canbus::Chassis>> chassis_reader_ = nullptr;
};

} // namespace gnss
} // namespace drivers
} // namespace apollo

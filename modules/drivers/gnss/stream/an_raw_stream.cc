#include <cmath>
#include <ctime>
#include <memory>
#include <thread>
#include <vector>

#include "absl/strings/str_cat.h"

#include "cyber/cyber.h"
#include "modules/common/adapters/adapter_gflags.h"
#include "modules/common/util/message_util.h"
#include "modules/drivers/gnss/proto/config.pb.h"
#include "modules/drivers/gnss/stream/an_raw_stream.h"
#include "modules/drivers/gnss/stream/stream.h"

namespace apollo {
namespace drivers {
namespace gnss {

void switch_stream_status(const apollo::drivers::gnss::Stream::Status &status,
                          StreamStatus_Type *report_status_type ) 
{
  switch (status) 
  {
    case apollo::drivers::gnss::Stream::Status::CONNECTED:
      *report_status_type = StreamStatus::CONNECTED;
      break;

    case apollo::drivers::gnss::Stream::Status::DISCONNECTED:
      *report_status_type = StreamStatus::DISCONNECTED;
      break;

    case apollo::drivers::gnss::Stream::Status::ERROR:

    default:
      *report_status_type = StreamStatus::DISCONNECTED;
      break;
  }
}

Stream *create_stream(const config::Stream &sd) 
{
  switch (sd.type_case()) 
  {
    case config::Stream::kSerial:
      if (!sd.serial().has_device()) 
      {
        AERROR << "Serial def has no device field.";
        return nullptr;
      }
      if (!sd.serial().has_baud_rate()) 
      {
        AERROR << "Serial def has no baud_rate field. Use default baud rate "
               << sd.serial().baud_rate();
        return nullptr;
      }
      return Stream::create_serial(sd.serial().device().c_str(),
                                   sd.serial().baud_rate() );

    default:
      return nullptr;
  }
}

AnRawStream::AnRawStream(const config::Config &config,
                         const std::shared_ptr<apollo::cyber::Node> &node )
  : config_(config), 
    node_(node) 
{
  data_parser_ptr_.reset(new DataParser(config_, node_));
  // rtcm_parser_ptr_.reset(new RtcmParser(config_, node_)); // Disalbed for syn
}

AnRawStream::~AnRawStream() 
{
  // this->Logout();
  this->Disconnect();

  // Disabled for syn
  // if (gpsbin_stream_ != nullptr) 
  // { gpsbin_stream_->close(); }

  if (data_thread_ptr_ != nullptr && data_thread_ptr_->joinable()) 
  { data_thread_ptr_->join(); }

  // Disabled for syn
  // if (rtk_thread_ptr_ != nullptr && rtk_thread_ptr_->joinable()) 
  // { rtk_thread_ptr_->join(); }
}

bool AnRawStream::Init() 
{
  CHECK_NOTNULL(data_parser_ptr_);
  // CHECK_NOTNULL(rtcm_parser_ptr_); // Disabked for syn
  if (!data_parser_ptr_->Init()) 
  {
    AERROR << "Init data parser failed.";
    return false;
  }

  // if (!rtcm_parser_ptr_->Init()) 
  // {
  //   AERROR << "Init rtcm parser failed.";
  //   return false;
  // }

  stream_status_.set_ins_stream_type(StreamStatus::DISCONNECTED);
  // stream_status_.set_rtk_stream_in_type(StreamStatus::DISCONNECTED);
  // stream_status_.set_rtk_stream_out_type(StreamStatus::DISCONNECTED);
  stream_writer_ = node_->CreateWriter<StreamStatus>(FLAGS_stream_status_topic);

  common::util::FillHeader("gnss", &stream_status_);
  stream_writer_->Write(stream_status_);

  // Creates streams.
  Stream *s = nullptr;
  if (!config_.has_data()) 
  {
    AINFO << "Error: Config file must provide the data stream.";
    return false;
  }

  s = create_stream(config_.data());
  if (s == nullptr) 
  {
    AERROR << "Failed to create data stream.";
    return false;
  }
  data_stream_.reset(s);

  Status *status = new Status();
  if (!status) 
  {
    AERROR << "Failed to create data stream status.";
    return false;
  }
  data_stream_status_.reset(status);
  
  // Disabled for syn
  // if (config_.has_command()) 
  // {
  //   s = create_stream(config_.command());
  //   if (s == nullptr) 
  //   {
  //     AERROR << "Failed to create command stream.";
  //     return false;
  //   }
  //   command_stream_.reset(s);

  //   status = new Status();
  //   if (!status) 
  //   {
  //     AERROR << "Failed to create command stream status.";
  //     return false;
  //   }
  //   command_stream_status_.reset(status);
  // } 
  // else 
  // {
  //   command_stream_ = data_stream_;
  //   command_stream_status_ = data_stream_status_;
  // }

  // Disabled for syn
  // if (config_.has_rtk_from()) 
  // {
  //   s = create_stream(config_.rtk_from());
  //   if (s == nullptr) 
  //   {
  //     AERROR << "Failed to create rtk_from stream.";
  //     return false;
  //   }
  //   in_rtk_stream_.reset(s);

  //   if (config_.rtk_from().has_push_location()) 
  //   {
  //     push_location_ = config_.rtk_from().push_location();
  //   }

  //   status = new Status();
  //   if (!status) 
  //   {
  //     AERROR << "Failed to create rtk_from stream status.";
  //     return false;
  //   }
  //   in_rtk_stream_status_.reset(status);

  //   if (config_.has_rtk_to()) 
  //   {
  //     s = create_stream(config_.rtk_to());
  //     if (s == nullptr) 
  //     {
  //       AERROR << "Failed to create rtk_to stream.";
  //       return false;
  //     }
  //     out_rtk_stream_.reset(s);

  //     status = new Status();
  //     if (!status) 
  //     {
  //       AERROR << "Failed to create rtk_to stream status.";
  //       return false;
  //     }
  //     out_rtk_stream_status_.reset(status);
  //   }
  //   else 
  //   {
  //     out_rtk_stream_ = data_stream_;
  //     out_rtk_stream_status_ = data_stream_status_;
  //   }

  // Disabled for syn
  //   if (config_.has_rtk_solution_type()) 
  //   {
  //     if (config_.rtk_solution_type() == config::Config::RTK_SOFTWARE_SOLUTION) 
  //     {
  //       rtk_software_solution_ = true;
  //     }
  //   }
  // }

  // if (config_.login_commands().empty()) 
  // { AWARN << "No login_commands in config file."; }

  // if (config_.logout_commands().empty()) 
  // { AWARN << "No logout_commands in config file."; }

  // connect and login
  if (!Connect()) 
  {
    AERROR << "gnss driver connect failed.";
    return false;
  }

  // if (!Login()) 
  // {
  //   AERROR << "gnss driver login failed.";
  //   return false;
  // }

  // const std::string gpsbin_file = getLocalTimeFileStr(config_.gpsbin_folder()); // Disabled for syn
  // gpsbin_stream_.reset(new std::ofstream(gpsbin_file, std::ios::app | std::ios::out | std::ios::binary)); // Disabled for syn
  stream_writer_ = node_->CreateWriter<StreamStatus>(FLAGS_stream_status_topic);
  raw_writer_ = node_->CreateWriter<RawData>(FLAGS_gnss_raw_data_topic);
  // rtcm_writer_ = node_->CreateWriter<RawData>(FLAGS_rtcm_data_topic);
  cyber::ReaderConfig reader_config;
  reader_config.channel_name = FLAGS_gnss_raw_data_topic;
  reader_config.pending_queue_size = 100;
  gpsbin_reader_ = node_->CreateReader<RawData>(reader_config, 
                                                [&] 
                                                (const std::shared_ptr<RawData> &raw_data) 
                                                { GpsbinCallback(raw_data); } );
  // Disabled for syn
  // chassis_reader_ = node_->CreateReader<Chassis>(FLAGS_chassis_topic, 
  //                                                [&] 
  //                                                (const std::shared_ptr<Chassis> &chassis) 
  //                                                { chassis_ptr_ = chassis; });

  return true;
}

void AnRawStream::Start() 
{
  data_thread_ptr_.reset(new std::thread(&AnRawStream::DataSpin, this));
  // rtk_thread_ptr_.reset(new std::thread(&RawStream::RtkSpin, this)); // Disabled for syn
  // Disabled for syn
  // if (config_.has_wheel_parameters()) 
  // {
  //   wheel_velocity_timer_.reset(new cyber::Timer(1000, 
  //                                                [this] 
  //                                                () 
  //                                                { this->OnWheelVelocityTimer(); }, 
  //                                                false ) );
  //   wheel_velocity_timer_->Start();
  // }

}

bool AnRawStream::Connect() 
{
  if (data_stream_) 
  {
    if (data_stream_->get_status() != Stream::Status::CONNECTED) 
    {
      if (!data_stream_->Connect()) 
      {
        AERROR << "data stream connect failed.";
        return false;
      }
      data_stream_status_->status = Stream::Status::CONNECTED;
      stream_status_.set_ins_stream_type(StreamStatus::CONNECTED);
    }
  }

  // if (command_stream_) 
  // {
  //   if (command_stream_->get_status() != Stream::Status::CONNECTED) 
  //   {
  //     if (!command_stream_->Connect()) 
  //     {
  //       AERROR << "command stream connect failed.";
  //       return false;
  //     }
  //     command_stream_status_->status = Stream::Status::CONNECTED;
  //   }
  // }

  // Disabled for syn
  // if (in_rtk_stream_) 
  // {
  //   if (in_rtk_stream_->get_status() != Stream::Status::CONNECTED) 
  //   {
  //     if (!in_rtk_stream_->Connect()) {
  //       AERROR << "in rtk stream connect failed.";
  //     } 
  //     else 
  //     {
  //       in_rtk_stream_status_->status = Stream::Status::CONNECTED;
  //       stream_status_.set_rtk_stream_in_type(StreamStatus::CONNECTED);
  //     }
  //   }
  // } 
  // else 
  // {
  //   stream_status_.set_rtk_stream_in_type(StreamStatus::CONNECTED);
  // }

  // Disabled for syn
  // if (out_rtk_stream_) {
  //   if (out_rtk_stream_->get_status() != Stream::Status::CONNECTED) {
  //     if (!out_rtk_stream_->Connect()) {
  //       AERROR << "out rtk stream connect failed.";
  //     } else {
  //       out_rtk_stream_status_->status = Stream::Status::CONNECTED;
  //       stream_status_.set_rtk_stream_out_type(StreamStatus::CONNECTED);
  //     }
  //   }
  // } else {
  //   stream_status_.set_rtk_stream_out_type(StreamStatus::CONNECTED);
  // }

  return true;
}

bool AnRawStream::Disconnect() 
{
  if (data_stream_) 
  {
    if (data_stream_->get_status() == Stream::Status::CONNECTED) 
    {
      if (!data_stream_->Disconnect()) 
      {
        AERROR << "data stream disconnect failed.";
        return false;
      }
    }
  }

  // Disabled for syn
  // if (command_stream_) {
  //   if (command_stream_->get_status() == Stream::Status::CONNECTED) {
  //     if (!command_stream_->Disconnect()) {
  //       AERROR << "command stream disconnect failed.";
  //       return false;
  //     }
  //   }
  // }
  // if (in_rtk_stream_) {
  //   if (in_rtk_stream_->get_status() == Stream::Status::CONNECTED) {
  //     if (!in_rtk_stream_->Disconnect()) {
  //       AERROR << "in rtk stream disconnect failed.";
  //       return false;
  //     }
  //   }
  // }
  // if (out_rtk_stream_) {
  //   if (out_rtk_stream_->get_status() == Stream::Status::CONNECTED) {
  //     if (!out_rtk_stream_->Disconnect()) {
  //       AERROR << "out rtk stream disconnect failed.";
  //       return false;
  //     }
  //   }
  // }

  return true;
}

void AnRawStream::StreamStatusCheck() 
{
  bool status_report = false;
  StreamStatus_Type report_stream_status;

  if (data_stream_ 
      && (data_stream_->get_status() != data_stream_status_->status) ) 
  {
    data_stream_status_->status = data_stream_->get_status();
    status_report = true;
    switch_stream_status(data_stream_status_->status, &report_stream_status);
    stream_status_.set_ins_stream_type(report_stream_status);
  }

  // Disabled for syn
  // if (in_rtk_stream_ &&
  //     (in_rtk_stream_->get_status() != in_rtk_stream_status_->status)) {
  //   in_rtk_stream_status_->status = in_rtk_stream_->get_status();
  //   status_report = true;
  //   switch_stream_status(in_rtk_stream_status_->status, &report_stream_status);
  //   stream_status_.set_rtk_stream_in_type(report_stream_status);
  // }

  // Disabled for syn
  // if (out_rtk_stream_ &&
  //     (out_rtk_stream_->get_status() != out_rtk_stream_status_->status)) {
  //   out_rtk_stream_status_->status = out_rtk_stream_->get_status();
  //   status_report = true;
  //   switch_stream_status(out_rtk_stream_status_->status, &report_stream_status);
  //   stream_status_.set_rtk_stream_out_type(report_stream_status);
  // }

  if (status_report) 
  {
    common::util::FillHeader("gnss", &stream_status_);
    stream_writer_->Write(stream_status_);
  }
}

void AnRawStream::DataSpin() 
{
  common::util::FillHeader("gnss", &stream_status_);
  stream_writer_->Write(stream_status_);
  while (cyber::OK()) 
  {
    size_t length = data_stream_->read(buffer_, BUFFER_SIZE);
    if (length > 0) 
    {
      std::shared_ptr<RawData> msg_pub = std::make_shared<RawData>();
      if (!msg_pub) 
      {
        AERROR << "New data sting msg failed.";
        continue;
      }
      msg_pub->set_data(reinterpret_cast<const char *>(buffer_), length);
      raw_writer_->Write(msg_pub);
      data_parser_ptr_->ParseRawData(msg_pub->data());
      
      // Disabled for syn
      // if (push_location_) 
      // { PushGpgga(length); }
    }
    StreamStatusCheck(); // Disabled for syn
  }
}

void AnRawStream::GpsbinCallback(const std::shared_ptr<RawData const> &raw_data) 
{
  if (gpsbin_stream_ == nullptr) 
  {
    return;
  }
  gpsbin_stream_->write(raw_data->data().c_str(), raw_data->data().size());
}

} // namespace gnss
} // namespace drivers
} // namespace apollo

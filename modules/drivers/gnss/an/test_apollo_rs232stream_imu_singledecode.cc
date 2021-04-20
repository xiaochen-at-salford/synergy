// Test if we can decode individual items rather than the system state one

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <unistd.h>

#include "cyber/cyber.h"
#include "cyber/time/rate.h"

#include "modules/drivers/gnss/an/rs232/rs232.h"
#include "modules/drivers/gnss/an/an_packet_protocol.h"
#include "modules/drivers/gnss/an/spatial_packets.h"

#include "modules/drivers/gnss/stream/stream.h"
// using namespace apollo::cyber;

#define RADIANS_TO_DEGREES (180.0/M_PI)

using namespace apollo::drivers::gnss;

int main(int argc, char *argv[]) 
{
	apollo::cyber::Init(argv[0]);


	// printf("\nYour Advanced Navigation ROS driver is currently running\nPress Ctrl-C to interrupt\n");

	// Set up the COM port
	std::string com_port = std::string("/dev/ttyUSB0");
	int baud_rate = 115200; 

	// Here, we use Apollo's SerialClass for the AN's rs232 reading
	Stream* ss = Stream::create_serial(const_cast<char*>(com_port.c_str()), baud_rate);	
	// apollo::drivers::gnss::SerialStream(const_cast<char*>(com_port.c_str()), baud_rate) ss;	
	ss->Connect();

	// Still use the data messague from AN's C codes
	an_decoder_t an_decoder;
	an_packet_t *an_packet;
	// system_state_packet_t system_state_packet;
	angular_velocity_packet_t angular_velocity_packet;

	int bytes_received;

	apollo::cyber::Rate rate(30.0);

	an_decoder_initialise(&an_decoder);

	// Loop continuously, polling for packets
	while (apollo::cyber::OK())
	{
		if ((bytes_received = ss->read(an_decoder_pointer(&an_decoder), an_decoder_size(&an_decoder))) > 0)
		{
			// increment the decode buffer length by the number of bytes received //
			an_decoder_increment(&an_decoder, bytes_received);

			// decode all the packets in the buffer
			while ((an_packet = an_packet_decode(&an_decoder)) != NULL)
			{

        // printf("here\n");
				// system state packet
				if (an_packet->id == packet_id_angular_velocity)
				{

          printf("here\n");
					if(decode_angular_velocity_packet(&angular_velocity_packet, an_packet) == 0)
					{
						// IMU
						float imu_x = angular_velocity_packet.angular_velocity[0]; // These the same as the TWIST msg values
						// float imu_y = system_state_packet.angular_velocity[1];
						// float imu_z = system_state_packet.angular_velocity[2];
            printf("imu_x: %f\n",imu_x);
            // printf("imu_y: %f\n",imu_y);
            // printf("imu_z: %f\n",imu_z);
						// imu_msg.linear_acceleration.x=system_state_packet.body_acceleration[0];
						// imu_msg.linear_acceleration.y=system_state_packet.body_acceleration[1];
						// imu_msg.linear_acceleration.z=system_state_packet.body_acceleration[2];

					}
				}


				an_packet_free(&an_packet);
			}
		}
		rate.Sleep();
	}

}

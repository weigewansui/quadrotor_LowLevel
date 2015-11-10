#include <lcm/lcm-cpp.hpp>
#include <time.h>
#include "../lcmtypes/state_t.hpp"
#include "../lcmtypes/transmitter_t.hpp"
#include "../include/llp_nav.h"

#define READ_RC false

int main() {

	LLP_NAV nav;
	nav.open_port();

	//publish via lcm
	lcm::LCM lcm;
	if(!lcm.good())
		return 1;

	state_t imu_msg;
	transmitter_t rc_msg;

	//read imu data
	IMU_CALCDATA imu_data;
	RC_DATA rc_data;

	while(1) {

		// nav.open_port();
		imu_data = nav.read_imu();

		imu_msg.timestamp = time(NULL);

		//waiting for other sensors
		imu_msg.position[0] = 0;
		imu_msg.position[1] = 0;
		imu_msg.position[2] = 0;
	
		imu_msg.velocity[0] = imu_data.speed_x;
		imu_msg.velocity[1] = imu_data.speed_y;
		imu_msg.velocity[2] = imu_data.speed_z;
		
		imu_msg.accel[0] = imu_data.acc_x;
		imu_msg.accel[1] = imu_data.acc_y;
		imu_msg.accel[2] = imu_data.acc_z;

		imu_msg.attitude[0] = imu_data.angle_roll;
		imu_msg.attitude[1] = imu_data.angle_nick;
		imu_msg.attitude[2] = imu_data.angle_yaw;

		imu_msg.angular_velocity[0] = imu_data.angvel_roll;
		imu_msg.angular_velocity[1] = imu_data.angvel_nick;
		imu_msg.angular_velocity[2] = imu_data.angvel_yaw;

		imu_msg.angular_accel[0] = imu_data.acc_angle_roll;
		imu_msg.angular_accel[1] = imu_data.acc_angle_nick;
		imu_msg.angular_accel[2] = 0;


		if(READ_RC) {

			rc_data = nav.read_rc();
			rc_msg.timestamp = time(NULL);
			rc_msg.ch_in0 = rc_data.channels_in[0];
			rc_msg.ch_in1 = rc_data.channels_in[1];
			rc_msg.ch_in2 = rc_data.channels_in[2];
			rc_msg.ch_in3 = rc_data.channels_in[3];
			rc_msg.ch_in4 = rc_data.channels_in[4];
			rc_msg.ch_in7 = rc_data.channels_in[5];
			rc_msg.ch_in6 = rc_data.channels_in[6];
			rc_msg.ch_in7 = rc_data.channels_in[7];

			rc_msg.ch_out0 = rc_data.channels_out[0];
			rc_msg.ch_out1 = rc_data.channels_out[1];
			rc_msg.ch_out2 = rc_data.channels_out[2];
			rc_msg.ch_out3 = rc_data.channels_out[3];
			rc_msg.ch_out4 = rc_data.channels_out[4];
			rc_msg.ch_out5 = rc_data.channels_out[5];
			rc_msg.ch_out6 = rc_data.channels_out[6];
			rc_msg.ch_out7 = rc_data.channels_out[7];

			lcm.publish("transmitter", &rc_msg);

		}

		lcm.publish("state", &imu_msg);
	}

	return 0;
}
#include "include/llp_ctrl.h"
#include "include/llp_nav.h"

int main() {

	LLP_CTRL m;
	LLP_NAV n;

	CTRL_INPUT cmmd;

	cmmd.thrust = 0;
	cmmd.pitch = 0;
	cmmd.roll = 0;
	cmmd.yaw = -2048;

	cmmd.ctrl = 0xF; //don't need GPS

	/*
		ctrl is defined as following:

	    bit 0: pitch control enabled
        bit 1: roll control enabled
        bit 2: yaw control enabled
        bit 3: thrust control enabled
        bit 4: height control enabled
        bit 5: GPS position control enabled

        if want 
    */
	cmmd.chksum = cmmd.thrust + cmmd.pitch + cmmd.roll + cmmd.yaw + cmmd.ctrl + 0xAAAA;

	m.switch_motor();
	usleep(1e6);
	// m.switch_motor();
	// m.open_port();
	// while(1) {

		n.open_port();
		while(1){
			IMU_CALCDATA imu_cal = n.read_imu();
			cout<<"nick angle:"<<imu_cal.angle_nick<<endl;
			cout<<"Acc_x_calib: "<<imu_cal.acc_x_calib<<endl;
			cout<<"acc_x: "<<imu_cal.acc_x<<endl;
		}

		n.close_port();
	// }


	// m.close_port();

}
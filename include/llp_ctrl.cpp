#include "llp_ctrl.h"

/**
 * send command to serial port
 * @param cmmd a CTRL_INPUT type, should be constructed by user
 * and send command to serial port
 */

LLP_CTRL::LLP_CTRL() : LLP_PORT() {}
LLP_CTRL::LLP_CTRL(termios prt_settings, string dName, int dSettings, int bRate) : LLP_PORT(prt_settings, dName, dSettings, bRate) {}

void LLP_CTRL::send_command(CTRL_INPUT* cmmd) {
	// open_port();
	write(fd, ">*>di",5 );
	write(fd, cmmd, sizeof(*cmmd));
	// close_port();
}

/**
 * function for turning on and turning off the motor;
 * if motor is off, this function switch on the motor;
 * if on, switch off motor;
 */
void LLP_CTRL::switch_motor() {

	CTRL_INPUT cmmd;
	cmmd.thrust = 0;
	cmmd.pitch = 0;
	cmmd.roll = 0;
	cmmd.yaw = -2048;
	cmmd.ctrl = 0xF;
	cmmd.chksum = cmmd.thrust + cmmd.pitch + cmmd.roll + cmmd.yaw + cmmd.ctrl + 0xAAAA;

	open_port();
	cout<<"Ctrl fd after open: "<<fd<<endl;
	send_command(&cmmd);
	close_port();
	cout<<"Ctrl fd after closed: "<<fd<<endl;

}

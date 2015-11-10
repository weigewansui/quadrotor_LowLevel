#include <lcm/lcm-cpp.hpp>
#include <stdlib.h>
#include <thread>
#include <iostream>

#include "../lcmtypes/state_t.hpp"
#include "../lcmtypes/vicon_state_t.hpp"
#include "../lcmtypes/command_t.hpp"

#include "../include/llp_ctrl.h"
#include "../include/llp_nav.h"
#include "../include/filter_util.h"
#include "../include/util.h"

float x, y, z;
float vx, vy, vz;
float phi, theta, psi;
float v_phi, v_theta, v_psi;

/**
 * 1820 hovering
 * 1840 hovering little ascending
 * 1870 ascending smoothly
 * 1900 ascending good
 * 1920 good
 * 1980 little aggresive
 * 2200 little aggresive
 *
 * 1600 descending good
 */

float HEIGHT = 1.0;
int thrust_0 = 1820;
int GAIN_P_H = -150;
int GAIN_D_H = -100;
int saturation_up = 2300;
int saturation_down = 1700;

float X_DES = 0.0;
int roll_0 = 0;
int GAIN_P_ROLL = -20;
int GAIN_D_ROLL = -10;

float Y_DES = 0.0;
int pitch_0 = 0;
int GAIN_P_PITCH = -20;
int GAIN_D_PITCH = -10;

float YAW_DES = 0.0;
int yaw_0 = 0;
int GAIN_P_YAW = -30;
int GAIN_D_YAW = -10;

float dt;


class Handler 
{   

    public:
        ~Handler() {}
        void handleMessage(const lcm::ReceiveBuffer* rbuf,
                const std::string& chan, 
                const vicon_state_t* msg)
        {
            x = msg->position[0]/1000;
            y = msg->position[1]/1000;
            z = msg->position[2]/1000;

            vx = msg->velocity[0];
            vy = msg->velocity[1];
            vz = msg->velocity[2];

        }

};

//lcm stuffs
lcm::LCM lcm_obj;
LLP_CTRL m;
CTRL_INPUT cmmd;

command_t cmmd_msg;

Handler handlerObj;


void update_state_loop() {

    lcm_obj.subscribe("vicon_state", &Handler::handleMessage, &handlerObj);

    while(0==lcm_obj.handle()){

        usleep(1e4);     

    }
}

void send_command_loop() {

    while(true) {
    // cmmd.thrust = (int)(thrust_0 + (z - HEIGHT)*atoi(argv[0]) + vz*atoi(argv[1]));
    // Height control
        cmmd.thrust = (int)(thrust_0 + (z - HEIGHT) * GAIN_P_H + vz * GAIN_D_H);
        // cmmd.thrust = 1600;
        if(cmmd.thrust >= saturation_up) cmmd.thrust = saturation_up;
        if(cmmd.thrust <= saturation_down) cmmd.thrust = saturation_down;

        cmmd.pitch = (int)(pitch_0 + (x - X_DES) * GAIN_P_PITCH + vx * GAIN_D_PITCH);
        //(int)(pitch_0 + (pitch_angle - PITCH) * GAIN_P_PITCH + pitch_vel * GAIN_D_PITCH);
        cmmd.roll  = (int)(roll_0 + (y - Y_DES) * GAIN_P_ROLL + vy * GAIN_D_ROLL);
        //(int)(roll_0 + (roll_angle - ROLL) * GAIN_P_ROLL + roll_vel * GAIN_D_ROLL);
        cmmd.yaw = (int)(yaw_0 + (psi - YAW_DES));
        //(int)(yaw_0 + (yaw_angle - YAW) * GAIN_P_YAW + yaw_vel * GAIN_D_YAW);
        
        //enable height control here
        cmmd.ctrl = 0x8;
        cmmd.chksum = cmmd.thrust + cmmd.pitch + cmmd.roll + cmmd.yaw + cmmd.ctrl + 0xAAAA;

        m.send_command(&cmmd);
        usleep(1e4);
    }
}

void publish_cmmdMsg_loop() {

    while(true) {

    //send command via lcm
    cmmd_msg.timestamp = utime_now();
    cmmd_msg.thrust = cmmd.thrust;
    cmmd_msg.pitch = cmmd.pitch;
    cmmd_msg.roll = cmmd.roll;
    cmmd_msg.yaw = cmmd.yaw;

    lcm_obj.publish("cmmd_msg", &cmmd_msg);
    usleep(1e4);

    }

}

int main(int argc, char* argv[]) {

    // get state data from lcm channel
    if(!lcm_obj.good())
        return 1;

    m.switch_motor();
    usleep(1e6);

    m.open_port();

    std::thread first(update_state_loop);
    std::thread second(send_command_loop);
    std::thread third(publish_cmmdMsg_loop);

    first.join();
    third.join();
    second.join();

    return 0;

}
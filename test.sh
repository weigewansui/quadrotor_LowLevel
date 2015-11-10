#! /bin/bash

./bin/Vicon_state &
VICON_PID=$!

./bin/control_test &
CTRL_PID=$!

read KILL

kill $VICON_PID
kill $CTRL_PID
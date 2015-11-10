#! /bin/bash

./bin/navigation &
NAV_PID=$!

lcm-logger &
LCM_LOGGER_PID=$!

read KILL

kill $LCM_LOGGER_PID
kill $NAV_PID

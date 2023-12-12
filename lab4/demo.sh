#!/bin/sh

set -x
# set -e

rmmod -f mydev
insmod mydev.ko

./writer dANNY &
./reader 192.168.3.50 8000 /dev/lab4_led

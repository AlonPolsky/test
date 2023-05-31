#!/bin/sh


make
sudo insmod message_slot.ko
sudo mknod /dev/message_slot_0 c 235 0
sudo mknod /dev/message_slot_1 c 235 1
sudo chmod o+rw /dev/message_slot_0
sudo chmod o+rw /dev/message_slot_1
gcc -O3 -Wall -std=c11 message_sender.c -o message_sender
gcc -O3 -Wall -std=c11 message_reader.c -o message_reader
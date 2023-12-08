#!/bin/bash

# Command 1 -- The Course Code
sudo rsync -avz -e ssh ~/Interrupts_and_Bottom_Halfs/ root@192.168.167.18:/home/Interrupts_and_Bottom_Halfs

# Command 2 -- The Kernel Code
sudo rsync -avz -e ssh /home/nanogens/Odroid/ root@192.168.167.18:/home/odroid/Odroid


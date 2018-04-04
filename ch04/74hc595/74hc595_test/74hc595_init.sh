#!/bin/bash

gpio -g mode 6 output
gpio -g mode 13 output
gpio -g mode 19 output
gpio -g mode 26 output
gpio -g write 26 1

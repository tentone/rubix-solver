#!/bin/bash

if (( $EUID > 0 )); then
	echo " - Please run as root"
	exit
fi

echo " - Install build dependencies"
apt install build-essential git gcc cmake make libgtkglext1-dev libgtk2.0-dev

echo " - Done!"
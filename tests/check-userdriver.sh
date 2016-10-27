#!/bin/bash -

DEV="ventilo0"

if [ -z "$(ls /dev | grep $DEV)" ]; then
	echo "script: /dev/$DEV is not found."
	echo "script: Fan may have been disconnected. Run this script once again."
	exit 1
fi

sudo ../src/kernel-module/user-driver/dreamyfan custom.config

err=$?

if [ $err -eq 0 ]; then
	echo "script: Test passed." && exit
fi

echo "script: Failed with code ($err)."

if [ $err -eq 2 ]; then
	echo "script: Unable to open /dev/$DEV."
	if [ ! -f /dev/$DEV ]; then
		echo "script: '/dev/$DEV' does not exist."
	fi
elif [ $err -eq 3 ]; then
	echo "script: Unable to write on /dev/$DEV"
fi

exit 1
